package pl.edu.mimuw.weather.event;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/

import java.time.LocalDateTime;

public class WeatherStateEvent extends WeatherAppEvent {
	private final LocalDateTime timestamp;

	public WeatherStateEvent () {
		timestamp = LocalDateTime.now();
	}

	public LocalDateTime getTimestamp() {
		return this.timestamp;
	}

	@Override
	public String toString() {
		return "WeatherStateEvent(timestamp=" + this.getTimestamp() + ")";
	}

}
