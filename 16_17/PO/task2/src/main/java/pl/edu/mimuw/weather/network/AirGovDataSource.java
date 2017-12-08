package pl.edu.mimuw.weather.network;

import com.google.gson.JsonObject;
import io.reactivex.netty.RxNetty;
import pl.edu.mimuw.weather.event.WeatherAirQualityUpdateEvent;
import rx.Observable;

import java.util.Currency;

public class AirGovDataSource extends WeatherDataSource {

    private static final String BASE_URL = "http://powietrze.gios.gov.pl/pjp/current/getAQIDetailsList?param=AQI";
    private static final int STATION_ID = 544;

    private Currency currency;

    public AirGovDataSource () {
    }

    @Override
    protected Observable <WeatherAirQualityUpdateEvent> makeRequest () {
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
                .map(JsonHelper::asJsonArray) // convert this string to a
                // JsonObject
                .map(jsonArray -> {
                    JsonObject station;
                    try {
                        station = JsonHelper.findElement(jsonArray, "stationId", STATION_ID);
                    } catch (NoSuchFieldException e) {
                        throw new RuntimeException("Station of a given ID not found", e);
                    }
                    return new WeatherAirQualityUpdateEvent(
                            /*
                             * Toggle the comments below if you want to see the
							 * rates changing. This adds some random bias to the
							 * actual data that may not change very often. For
							 * development purposes only.
							 */
                            station.get("values").getAsJsonObject().get("PM2.5").getAsFloat()
//                                    + (0.5f - (float) Math.random())
                            ,
                            station.get("values").getAsJsonObject().get("PM10").getAsFloat()
//									+ (0.5f - (float) Math.random())

                    );
                });
    }

}
