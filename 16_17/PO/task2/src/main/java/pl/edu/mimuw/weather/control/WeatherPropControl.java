package pl.edu.mimuw.weather.control;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.text.Text;
import org.kordamp.ikonli.fontawesome.FontAwesome;
import org.kordamp.ikonli.javafx.FontIcon;
import org.kordamp.ikonli.weathericons.WeatherIcons;
import pl.edu.mimuw.weather.event.RawValueEvent;
import rx.Observable;

import java.text.DecimalFormat;

public class WeatherPropControl extends Pane {

    private FontIcon upIcon;
    private FontIcon downIcon;
    private FontIcon currentIcon;

    private FontIcon noDataIcon = new FontIcon(WeatherIcons.NA);

    private HBox innerContainer;

    private Text prefixLabel;
    private Text suffixLabel;
    private Text textControl;

    private ObjectProperty <Observable <RawValueEvent>> sourceProperty = new SimpleObjectProperty <>();

    private String formatPattern = "0.###";
    private DecimalFormat format = new DecimalFormat(formatPattern);

    private StringProperty prefixProperty = new SimpleStringProperty();
    private StringProperty suffixProperty = new SimpleStringProperty("");
    private StringProperty titleProperty = new SimpleStringProperty("-");

    public Observable <RawValueEvent> getSource () {
        return sourceProperty.get();
    }

    @FXML
    public void setSource (Observable <RawValueEvent> source) {
        source.subscribe(e -> {
            if (e.getValue().isNaN()) {
                setControlsAsUnavailable();
            } else {
                createContentControls();

                textControl.setText(format.format(e.getValue()));
            }
        });

        sourceProperty.set(source);
    }

    public String getFormat () {
        return formatPattern;
    }

    public void setFormat (String pattern) {
        formatPattern = pattern;
        format = new DecimalFormat(pattern);
    }

    public String getPrefix () {
        return prefixProperty.get();
    }

    public void setPrefix (String prefix) {
        prefixProperty.set(prefix);
    }

    public String getSuffix () {
        return suffixProperty.get();
    }

    public void setSuffix (String sufix) {
        suffixProperty.set(sufix);
    }

    public String getTitle () {
        return titleProperty.get();
    }

    public void setTitle (String title) {
        titleProperty.set(title);
    }

    public WeatherPropControl () {
        noDataIcon.getStyleClass().add("no-data");
        getChildren().add(noDataIcon);
    }

    private void createContentControls () {
        getChildren().removeAll(noDataIcon);
        getChildren().removeAll(innerContainer);

        textControl = new Text();
        textControl.getStyleClass().add("rate-value");

        prefixLabel = new Text();
        prefixLabel.textProperty().bind(prefixProperty);
        prefixLabel.getStyleClass().add("helper-label");

        suffixLabel = new Text();
        suffixLabel.textProperty().bind(suffixProperty);
        suffixLabel.getStyleClass().add("helper-label");

        innerContainer = new HBox();
        innerContainer.getStyleClass().add("value-container");
        innerContainer.getChildren().addAll(prefixLabel, textControl, suffixLabel);

        getChildren().add(innerContainer);

        upIcon = new FontIcon(FontAwesome.CHEVRON_UP);
        upIcon.getStyleClass().add("chevron-up");
        downIcon = new FontIcon(FontAwesome.CHEVRON_DOWN);
        downIcon.getStyleClass().add("chevron-down");
    }

    private void setControlsAsUnavailable () {
        getChildren().removeAll(noDataIcon);
        getChildren().removeAll(innerContainer);

        noDataIcon.getStyleClass().add("no-data");
        getChildren().add(noDataIcon);
    }

    //todo implement rotating wind icon
    //todo implement weather icon view

    @Override
    protected void layoutChildren () {
        /* Custom children positioning */
        super.layoutChildren();

        if (noDataIcon.isVisible()) {
            noDataIcon.relocate((getWidth() - noDataIcon.getLayoutBounds().getWidth()) / 2,
                    (getHeight() - noDataIcon.getLayoutBounds().getHeight()) / 2);
        }

        if (innerContainer != null) {
            innerContainer.relocate((getWidth() - innerContainer.getLayoutBounds().getWidth()) / 2,
                    (getHeight() - innerContainer.getLayoutBounds().getHeight()) / 2);
        }

        if (currentIcon != null) {
            currentIcon.relocate(
                    suffixLabel.getBoundsInParent().getMinX() + innerContainer.getBoundsInParent().getMinX(),
                    suffixLabel.getBoundsInParent().getMinY() + innerContainer.getBoundsInParent().getMinY()
                            - currentIcon.getLayoutBounds().getHeight() + 2);
        }

    }

}
