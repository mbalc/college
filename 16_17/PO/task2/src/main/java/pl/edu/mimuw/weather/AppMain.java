package pl.edu.mimuw.weather;

import com.jfoenix.controls.JFXDecorator;
import com.jfoenix.controls.JFXDialog;
import com.jfoenix.controls.JFXRadioButton;
import com.jfoenix.controls.JFXTextField;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ToggleGroup;
import javafx.scene.image.Image;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.util.Duration;
import pl.edu.mimuw.weather.control.TooltipProlongHelper;
import pl.edu.mimuw.weather.event.SettingsRequestEvent;
import pl.edu.mimuw.weather.event.WeatherAppEvent;
import pl.edu.mimuw.weather.event.WeatherStateEvent;
import pl.edu.mimuw.weather.network.*;
import rx.Observable;
import rx.Subscription;
import rx.observables.JavaFxObservable;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

import static pl.edu.mimuw.weather.event.EventStream.eventStream;
import static pl.edu.mimuw.weather.event.EventStream.joinStream;


public class AppMain extends Application {
    private static final org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(AppMain.class);

    private static final String FXML_MAIN_FORM_TEMPLATE = "/fxml/weather-main.fxml";
    private static final String FXML_CLOSE_DIALOG_TEMPLATE = "/fxml/close-weather-dialog.fxml";
    private static final String FXML_SETTINGS_DIALOG_TEMPLATE = "/fxml/settings-dialog.fxml";

    private static final String FONT_CSS = "/css/jfoenix-fonts.css";
    private static final String MATERIAL_CSS = "/css/jfoenix-design.css";
    private static final String JFX_CSS = "/css/jfx.css";

    private static final int DEFAULT_SOURCE_NUMBER = 0;
    private static int sourceNumberChoice;

    private static final String[] LOGOS = {"/icons/bitcoin16.png", "/icons/bitcoin24.png", "/icons/bitcoin32.png"};


    private static WeatherDataSource[] dataSources;
    private static WeatherDataSource myDataSource;

    private static void setSourceNumberChoice (int sourceNumberChoice) {
        AppMain.sourceNumberChoice = sourceNumberChoice;
    }

    private class DialogControllerBase {
        @FXML
        JFXDialog dialog;

        @FXML
        Button acceptButton;

        @FXML
        Button cancelButton;

        void initialize () {
            JavaFxObservable.actionEventsOf(cancelButton).subscribe(ignore -> {
                dialog.close();
            });
        }

        void show (StackPane pane) {
            dialog.show(pane);
        }

    }

    private class CloseDialogController extends DialogControllerBase {
        @FXML
        void initialize () {
            super.initialize();

            JavaFxObservable.actionEventsOf(acceptButton).subscribe(ignore -> {
                log.info("Exiting...");
                AppMain.this.mainStage.close(); // This should terminate the
                // application
                System.exit(0); // Just for sure
            });
        }
    }

    private class SettingsDialogController extends DialogControllerBase {
        @FXML
        JFXTextField intervalField;

        @FXML
        Pane content;

        @FXML
        JFXRadioButton openToogle;

        @FXML
        JFXRadioButton meteoToogle;


        @FXML
        void initialize () {
            super.initialize();

            final ToggleGroup group = new ToggleGroup();

            openToogle.setPadding(new Insets(6));
            openToogle.setToggleGroup(group);

            meteoToogle.setPadding(new Insets(6));
            meteoToogle.setToggleGroup(group);

            intervalField.textProperty().addListener((control, newValue, oldValue) -> intervalField.validate());
            acceptButton.disableProperty().bind(intervalField.getValidators().get(0).hasErrorsProperty());

            JavaFxObservable.actionEventsOf(openToogle).subscribe(ignore -> setSourceNumberChoice(0));
            JavaFxObservable.actionEventsOf(meteoToogle).subscribe(ignore -> setSourceNumberChoice(1));

            JavaFxObservable.actionEventsOf(acceptButton).subscribe(ignore -> {
                try {
                    myDataSource = dataSources[sourceNumberChoice];

                    AppMain.this.sourceStreams.stream().forEach(Subscription::unsubscribe);
                    AppMain.this.sourceStreams.clear();
                    if (intervalField.getText().equals("")) AppMain.this.setupDataSources(null);
                    else AppMain.this.setupDataSources(Integer.parseInt(intervalField.getText()));
                } finally {
                    dialog.close();
                }
            });

        }

    }

    private DialogControllerBase closeDialogController;
    private DialogControllerBase settingsDialogController;
    private Stage mainStage;

    private List <Subscription> sourceStreams = new LinkedList <>();

    @Override
    public void start (Stage primaryStage) throws Exception {

        dataSources = new WeatherDataSource[] {new OpenWeatherDataSource(), new MeteoWeatherDataSource()};
        sourceNumberChoice = DEFAULT_SOURCE_NUMBER;
        myDataSource = dataSources[sourceNumberChoice];
        log.info("Starting Weather application...");

        mainStage = primaryStage;

        setupTooltipDuration();

        setupDataSources(null);

        setupEventHandler();

        Parent pane = FXMLLoader.load(AppMain.class.getResource(FXML_MAIN_FORM_TEMPLATE));
        /*
         * Transform the main stage (aka the main window) into an undecorated
		 * window
		 */
        JFXDecorator decorator = new JFXDecorator(mainStage, pane, false, false, true);
        ObservableList <Node> buttonsList = ((Pane) decorator.getChildren().get(0)).getChildren();
        buttonsList.get(buttonsList.size() - 1).getStyleClass().add("close-button"); // Style
        // the
        // close
        // button
        // differently

        decorator.setOnCloseButtonAction(this::onClose);

        Scene scene = new Scene(decorator);
        scene.setFill(null);

        scene.getStylesheets().addAll(AppMain.class.getResource(FONT_CSS).toExternalForm(),
                AppMain.class.getResource(MATERIAL_CSS).toExternalForm(),
                AppMain.class.getResource(JFX_CSS).toExternalForm());

        mainStage.setScene(scene);

        mainStage.setWidth(650);
        mainStage.setHeight(250);
        mainStage.setResizable(false);

        addLogo();

        mainStage.show();

        log.info("Application's up and running!");
    }

    private void addLogo () {
        for (String logoPath : LOGOS) {
            mainStage.getIcons().add(new Image(AppMain.class.getResourceAsStream(logoPath)));
        }
    }

    private void onClose () {
        log.info("onClose");

        if (closeDialogController == null) {
            closeDialogController = new CloseDialogController();
            createDialog(closeDialogController, FXML_CLOSE_DIALOG_TEMPLATE);
        }

        closeDialogController.show(getMainPane());
    }

    private void onSettingsRequested () {
        log.info("onSettingsRequested");

        if (settingsDialogController == null) {
            settingsDialogController = new SettingsDialogController();
            createDialog(settingsDialogController, FXML_SETTINGS_DIALOG_TEMPLATE);
        }

        settingsDialogController.show(getMainPane());
    }

    private StackPane getMainPane () {
        return (StackPane) mainStage.getScene().getRoot().lookup("#main");
    }

    private void createDialog (Object dialogController, String fxmlPath) {
        FXMLLoader loader = new FXMLLoader(AppMain.class.getResource(fxmlPath));
        loader.setController(dialogController);
        try {
            loader.load();
        } catch (IOException e) {
            log.error(e);
            throw new RuntimeException(e);
        }
    }

    //todo implement data source switcher support

    private void setupDataSources (Integer pollInterval) {
        WeatherDataSource[] sources = {myDataSource,
                new AirGovDataSource(), new OpenWeatherWeatherIconSource()};
        for (WeatherDataSource source : sources) {
            sourceStreams.add(joinStream(source.dataSourceStream(pollInterval)));
        }
    }

    private void setupEventHandler () {
        Observable <WeatherAppEvent> events = eventStream().events();

        events.ofType(WeatherStateEvent.class).subscribe(log::info);

        events.ofType(SettingsRequestEvent.class).subscribe(e -> onSettingsRequested());
    }

    private static void setupExceptionHandler () {
        Thread.setDefaultUncaughtExceptionHandler(
                (t, e) -> log.error("Uncaught exception in thread \'" + t.getName() + "\'", e));
    }

    private static void setupTooltipDuration () {
        TooltipProlongHelper.setTooltipDuration(Duration.millis(500), Duration.minutes(10), Duration.millis(500));
    }

    private static void setupTextRendering () {
        /*
         * A workaround for the font rendering issue on some platforms.
		 * Consult: @link{https://stackoverflow.com/questions/18382969/can-the-
		 * rendering-of-the-javafx-2-8-font-be-improved} and linked materials
		 */
        System.setProperty("prism.text", "t2k");
        System.setProperty("prism.lcdtext", "true");
    }

    public static void main (String[] args) {
        setupExceptionHandler();

        setupTextRendering();

        Platform.setImplicitExit(true); // This should exit the application when
        // the main window gets closed
        Application.launch(AppMain.class, args);
    }
}
