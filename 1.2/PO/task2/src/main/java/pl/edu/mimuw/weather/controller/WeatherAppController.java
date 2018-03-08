package pl.edu.mimuw.weather.controller;

import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Tooltip;
import pl.edu.mimuw.weather.control.WeatherPropControl;
import pl.edu.mimuw.weather.control.WeatherTooltip;
import pl.edu.mimuw.weather.event.*;
import rx.Observable;
import rx.functions.Func1;
import rx.observables.JavaFxObservable;
import rx.schedulers.JavaFxScheduler;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.text.MessageFormat;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.TimeUnit;

import static pl.edu.mimuw.weather.event.EventStream.*;

public class WeatherAppController {


    public static final int DEFAULT_POLL_INTERVAL = 150;

    private static final int ERROR_MSG_MAX_LENGTH = 400;
    private static final int ERROR_MSG_DURATION = 30; // Show error icon for 30
    // seconds

    @FXML
    private WeatherPropControl temperatureControl;

    @FXML
    private WeatherPropControl cloudinessControl;

    @FXML
    private WeatherPropControl windForceControl;

    @FXML
    private WeatherPropControl windDirectionControl;

    @FXML
    private WeatherPropControl airPressureControl;

    @FXML
    private WeatherPropControl humidityControl;

    @FXML
    private WeatherPropControl pm025Control;

    @FXML
    private WeatherPropControl pm100Control;

    @FXML
    private Node errorIcon;

    @FXML
    private Node workingIcon;

    @FXML
    private Button refreshButton;

    @FXML
    private Button settingsButton;

    @FXML
    private void initialize () {
        initializeStatus();

        initalizeRefreshHandler();
        initializeSettingsHandler();

        initializeTooltips();

    }

    public Observable <RawValueEvent> getTemperature () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getTemperature);
    }

    public Observable <RawValueEvent> getCloudiness () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getCloudiness);
    }

    public Observable <RawValueEvent> getWindForce () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getWindForce);
    }

    public Observable <RawValueEvent> getWindDirection () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getWindDirection);
    }

    public Observable <RawValueEvent> getAirPressure () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getPressure);
    }

    public Observable <RawValueEvent> getHumidity () {
        return getWeatherValueStream(WeatherPropertiesUpdateEvent::getHumidity);
    }

    public Observable <RawValueEvent> getPm025 () {
        return getWeatherAirValueStream(WeatherAirQualityUpdateEvent::getPm025);
    }

    public Observable <RawValueEvent> getPm100 () {
        return getWeatherAirValueStream(WeatherAirQualityUpdateEvent::getPm100);
    }

    private void initalizeRefreshHandler () {
        joinStream(JavaFxObservable.actionEventsOf(refreshButton).map(e -> new RefreshRequestEvent()));
    }

    private void initializeSettingsHandler () {
        joinStream(JavaFxObservable.actionEventsOf(settingsButton).map(e -> new SettingsRequestEvent()));
    }

    private void initializeStatus () {
        Observable <WeatherAppEvent> events = eventStream().eventsInFx();

        // Basically, we keep track of the difference between issued requests
        // and completed requests
        // If this difference is > 0 we display the spinning icon...
        workingIcon.visibleProperty()
                .bind(binding(events.ofType(NetworkRequestIssuedEvent.class).map(e -> 1) // Every
                        // issued
                        // request
                        // contributes
                        // +1
                        .mergeWith(events.ofType(NetworkRequestFinishedEvent.class).map(e -> -1) // Every
                                // completed
                                // request
                                // contributes
                                // -1
                                .delay(2, TimeUnit.SECONDS, JavaFxScheduler.getInstance())) // We delay
                        // completion
                        // events for 2
                        // seconds so
                        // that the
                        // spinning icon
                        // is always
                        // displayed for
                        // at least 2
                        // seconds and
                        // it does not
                        // blink
                        .scan(0, (x, y) -> x + y).map(v -> v > 0))

                );

		/*
         * This should show the error icon when an error event arrives and hides
		 * the icon after 30 seconds unless another error arrives
		 */
        Observable <ErrorEvent> errors = events.ofType(ErrorEvent.class);
        errorIcon.visibleProperty()
                .bind(onEvent(errors, true).andOn(
                        errors.throttleWithTimeout(ERROR_MSG_DURATION, TimeUnit.SECONDS, JavaFxScheduler.getInstance()),
                        false).toBinding());
    }

    private void initializeTooltips () {
        Tooltip.install(workingIcon, new Tooltip("Fetching data..."));

        Tooltip errorTooltip = new Tooltip();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        eventStream().eventsInFx().ofType(ErrorEvent.class).subscribe(e -> {
            ByteArrayOutputStream ostream = new ByteArrayOutputStream();
            e.getCause().printStackTrace(new PrintStream(ostream));
            String details = new String(ostream.toByteArray());
            if (details.length() > ERROR_MSG_MAX_LENGTH) {
                details = details.substring(0, ERROR_MSG_MAX_LENGTH) + "\u2026"; // Add
                // ellipsis
                // (...)
                // at
                // the
                // end
            }

            errorTooltip.setText(MessageFormat.format("An error has occurred ({0}):\n{1}",
                    e.getTimestamp().format(formatter), details));
        });
        Tooltip.install(errorIcon, errorTooltip);

        WeatherPropControl[] weatherControls = {temperatureControl, windForceControl, windDirectionControl, airPressureControl,
                humidityControl, cloudinessControl, pm025Control, pm100Control};
        for (WeatherPropControl control : weatherControls) {
            Tooltip tooltipPopup = new Tooltip();
            WeatherTooltip tooltipContent = new WeatherTooltip(control.getSource(), control.getTitle());
//
            tooltipPopup.setGraphic(tooltipContent);

            Tooltip.install(control, tooltipPopup);
        }

    }

    private Observable <RawValueEvent> getWeatherValueStream (Func1 <WeatherPropertiesUpdateEvent, Float> extractor) {
        return eventStream().eventsInFx().ofType(WeatherPropertiesUpdateEvent.class)
                .map(e -> new RawValueEvent(e.getTimestamp(), extractor.call(e)));
    }

    private Observable <RawValueEvent> getWeatherAirValueStream (Func1 <WeatherAirQualityUpdateEvent, Float> extractor) {
        return eventStream().eventsInFx().ofType(WeatherAirQualityUpdateEvent.class)
                .map(e -> new RawValueEvent(e.getTimestamp(), extractor.call(e)));
    }

}
