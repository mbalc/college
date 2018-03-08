package pl.edu.mimuw.weather.control;

import javafx.beans.DefaultProperty;
import javafx.beans.Observable;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.ObjectPropertyBase;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.HPos;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.StackPane;

import java.io.IOException;
import java.util.Iterator;

@DefaultProperty ("rates")
public class WeatherPane extends StackPane {

    private static final org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(WeatherPane.class);

    private static final String FXML_TEMPLATE = "/fxml/weather-pane.fxml";

    @FXML
    private GridPane containerGrid;

    @FXML
    private StackPane imageContainer;

    private ObjectProperty <Node> image;
    private ObservableList <Node> rates = FXCollections.observableArrayList();

    public ObjectProperty <Node> imageProperty () {
        if (image == null) {
            image = new ObjectPropertyBase <Node>() {
                @Override
                public Object getBean () {
                    return WeatherPane.this;
                }

                @Override
                public String getName () {
                    return "image";
                }

                @Override
                protected void invalidated () {
                    final ObservableList <Node> children = imageContainer.getChildren();
                    if (children.size() > 0) {
                        children.clear();
                    }
                    children.add(get());
                    imageContainer.requestLayout();
                }
            };
        }
        return image;
    }

    public Node getImage () {
        return imageProperty().get();
    }

    public void setImage (Node image) {
        imageProperty().set(image);
    }

    public ObservableList <Node> getRates () {
        return rates;
    }

    public WeatherPane () throws IOException {
        final FXMLLoader loader = new FXMLLoader(WeatherPane.class.getResource(FXML_TEMPLATE));
        loader.setController(this);
        getChildren().add(loader.load());

        rates.addListener((Observable o) -> {
            int size = rates.size();
            int constraintsSize = containerGrid.getRowConstraints().size();
            if (size > constraintsSize) {
                log.error("Too many rates for WeatherPane!");
                return;
            }

            Iterator <Node> it = containerGrid.getChildren().iterator();
            while (it.hasNext()) {
                if (GridPane.getColumnIndex(it.next()) == 1) {
                    it.remove();
                }
            }

            containerGrid.getChildren().addAll(rates);

            int i = 0;
            for (Node n : rates) {
                GridPane.setColumnIndex(n, 1);
                GridPane.setRowIndex(n, i);
                GridPane.setRowSpan(n, 1);
                GridPane.setHalignment(n, HPos.LEFT);
                i++;
            }

            if (size < constraintsSize) {
                GridPane.setRowSpan(rates.get(size - 1), constraintsSize - i + 1);
            }
        });
    }
}
