package pl.edu.mimuw.weather.network;

import io.reactivex.netty.RxNetty;
import pl.edu.mimuw.weather.event.WeatherAppEvent;
import pl.edu.mimuw.weather.event.WeatherPropertiesUpdateEvent;
import rx.Observable;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class MeteoWeatherDataSource extends WeatherDataSource {

    private static final String URL = "http://www.meteo.waw.pl/";

    private static Matcher matcherMaker (String key, String src) {
        return Pattern.compile("<span\\s*id=\\\"" + key + "\\\">([NSWE\\d\\,\\.]*)</span>",
                Pattern.CASE_INSENSITIVE).matcher(src);
    }

    private static String degreeMatcher (String dir) {
        switch (dir) {
            case "N":
                return "0";
            case "E":
                return "90";
            case "S":
                return "180";
            case "W":
                return "270";
            case "NE":
                return "45";
            case "SE":
                return "135";
            case "SW":
                return "225";
            case "NW":
                return "315";
        }
        return dir;
    }

    private static String matcherStringPuller (Matcher arg) {

        if (arg.find()) {
            return arg.group(1).trim().replace(",", ".");
        } else return "NaN";

    }


    @Override

    protected Observable <WeatherAppEvent> makeRequest () {

		/*
         * Bitcoin data source is a different story. We chose to extract bitcoin
		 * rates directly from the HTML source of a Polish portal for bitcoin
		 * rather than use nice and clean REST API. This is just for
		 * demonstration.
		 *
		 * Now to the details. The webpage source looks like this:
		 *
		 * <div id="blankBM441" > <div id="holderBM441"> <div id="innerBM441">
		 * <div> </div> <div> <p class="price" style="font-family: Lato;"><img
		 * src="/images/price-tag.png" alt=Kurs bitcoina" />8410 <span >
		 * zł</span></p> </div> </div> </div> </div>
		 *
		 * This isn't even a valid HTML code (note the missing quotation mark
		 * after alt=). We are interested in extracting the "8410" substring
		 * from it. A regular expression that finds the <p class="price"> tag,
		 * then skips the <img> tag, and extracts digits immediately after it
		 * does its job.
		 *
		 * Oh, and webpages aren't normally meant to be processed using regular
		 * expressions. We could use a full-blown HTML parser, like jsoup
		 * (https://jsoup.org/), but that would be an overkill. Three lines of
		 * code with a cheeky regexp just work.
		 */
        return RxNetty.createHttpRequest(prepareHttpGETRequest(URL)).compose(this::unpackResponse)
                .map(htmlSource -> {
                    Matcher matcherArray[] = {
                            matcherMaker("PARAM_0_TA", htmlSource),
                            matcherMaker("PARAM_0_WDABBR", htmlSource),
                            matcherMaker("PARAM_0_WV", htmlSource),
                            matcherMaker("PARAM_0_RH", htmlSource),
                            matcherMaker("PARAM_0_PR", htmlSource),
                            matcherMaker("PARAM_0_RH_NON_EXISTENT_STRING", htmlSource)
                    };
                    Float returnArray[] = new Float[WeatherPropertiesUpdateEvent.PROPERTIES_COUNT];
                    for (int i = 0; i < WeatherPropertiesUpdateEvent.PROPERTIES_COUNT; ++i) {
                        returnArray[i] = Float
                                .parseFloat(
                                        degreeMatcher(
                                                matcherStringPuller(
                                                        matcherArray[i])
                                        ));
                    }
                    return new WeatherPropertiesUpdateEvent(returnArray);
                });
    }
}
