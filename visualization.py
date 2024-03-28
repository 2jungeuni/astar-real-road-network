import folium
import numpy as np
import pandas as pd

if __name__ == "__main__":
    # central coordinate of New York City
    central_lat, central_lng = 40.78554, -73.95956

    # get an optimal path
    optimal_path = pd.read_csv("./result/optimal_path.csv")

    # visualization
    m = folium.Map(location=(central_lat, central_lng), zoom_start=10)
    lat_path = list(optimal_path['latitude'])
    lng_path = list(optimal_path['longitude'])

    folium.PolyLine(locations=np.array([lat_path, lng_path]).T, smooth_factor=1.0, weight=2.0, color='blue').add_to(m)
    folium.Marker([lat_path[0], lng_path[0]], icon=folium.Icon(icon='none', color='blue')).add_to(m)
    folium.Marker([lat_path[-1], lng_path[-1]], icon=folium.Icon(icon='none', color='red')).add_to(m)

    m.save("./result/path.html")