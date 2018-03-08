package pl.edu.mimuw.weather.network;

import com.google.gson.JsonObject;
import io.reactivex.netty.RxNetty;
import pl.edu.mimuw.weather.event.WeatherIconUpdateEvent;
import rx.Observable;

public class OpenWeatherWeatherIconSource extends WeatherDataSource {

    private static final String BASE_URL = "http://api.openweathermap.org/data/2.5/weather?id=756135&units=metric&APPID=bd5e378503939ddaee76f12ad7a97608";

    public OpenWeatherWeatherIconSource () {
    }

    private Float myPropGetter (JsonObject src, String key1, String key2) {
        try {
            return src.get(key1).getAsJsonObject().get(key2).getAsFloat();
        } catch (NullPointerException e) {
            return Float.NaN;
        }
    }

    @Override
    protected Observable <WeatherIconUpdateEvent> makeRequest () {
        String url = BASE_URL;

		/*
         * This issues REST queries and transforms received JSONs into a stream
		 * of exchange rate events. We have to transform something of this form
		 * (a reply from NBP's REST API):
		 *
		 * {"table":"C","currency":"euro","code":"EUR","rates":[{"no":
		 * "101/C/NBP/2017","effectiveDate":"2017-05-26","bid":4.1329,"ask":4.
		 * 2163}]}
		 *
		 * into a Java object that looks like this:
		 *
		 * CurrencyRateEvent(currency=EUR, bid=4.1329, ask=4.2163)
		 *
		 */

        return RxNetty
                .createHttpRequest( // create an HTTP request using RxNetty
                        JsonHelper.withJsonHeader(prepareHttpGETRequest(url)))
                // add an HTTP request header that instructs the server to
                // respond with JSON
                .compose(this::unpackResponse) // extract response body to a
                // string
                .map(JsonHelper::asJsonObject) // convert this string to a
                // JsonObject=
                .map(jsonObject -> { // extract ask/ bid prices
                    return new WeatherIconUpdateEvent(
                            "http://openweathermap.org/img/w/" +
                                    jsonObject.get("weather").getAsJsonArray()
                                            .get(0).getAsJsonObject()
                                            .get("icon").getAsString() +
                                    ".png"
                    );
                });
    }

}
