package pl.edu.mimuw.weather.event;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class WeatherIconUpdateEvent extends WeatherStateEvent {

    private final String iconUrl;

    @Override
    public String toString () {
        return "WeatherIconUpdateEvent{" +
                "iconUrl='" + iconUrl + '\'' +
                '}';
    }

    public String getIconUrl () {
        return iconUrl;
    }

    public WeatherIconUpdateEvent (String iconUrl) {

        this.iconUrl = iconUrl;
    }
}
