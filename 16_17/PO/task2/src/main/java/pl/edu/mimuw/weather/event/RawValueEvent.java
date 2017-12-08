package pl.edu.mimuw.weather.event;

import java.time.LocalDateTime;

public final class RawValueEvent extends WeatherAppEvent {
	private final LocalDateTime timestamp;
	private final Float value;

	public RawValueEvent (final LocalDateTime timestamp, final float value) {
		this.timestamp = timestamp;
		this.value = value;
	}

	public LocalDateTime getTimestamp() {
		return this.timestamp;
	}

	public Float getValue() {
		return this.value;
	}

	@Override
	public String toString() {
		return "RawRateEvent(timestamp=" + this.getTimestamp() + ", value=" + this.getValue() + ")";
	}

}
