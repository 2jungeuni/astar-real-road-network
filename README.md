## astar-real-road-network
Routing algorithm for real road networks, especially A*.
***
### :computer: Dataset
It uses New York City's actual road network.
The data comes from [1].
- There are 96,570 nodes.
- There are 260,855 links.
***
### :memo: Problem formulation
This algorithm generates a path considering three metrics.
- Street length (meters): There are pre-calculated street lengths from [1]. The heuristic function for distance metric uses Haversine distance.
- Travel time (seconds): It is calculated by dividing the distance by the speed limit from [3]; see [4] for more details.
- Carbon emission (g/m): It can be estimated based on elevation, speed, and acceleration as driving from the equation in [2]. <ins>The algorithm assumes all vehicles drive at the speed limit (i.e., a=0).</ins>  
$$f_t = \beta_1 \cos\theta + \beta_2 v \sin\theta + \beta_3 v^2 + \beta_4 a + \beta_5 \frac{a}{v} + \beta_6 \frac{1}{v} + \beta_7$$
$$E_{CO_2} = 2.32 \times f_t$$   
|parameter|value|  
|:---:|:---:|  
|a|b|
|제목|내용|설명|
|------|---|---|
|테스트1|테스트2|테스트3|
|테스트1|테스트2|테스트3|
|테스트1|테스트2|테스트3|
***
### :envelope: Python modules
pybind11
***
### :runner: Run demo code
***
### :earth_asia: Visualization
***
### :pushpin: References
[1] Donovan, Brian; Work, Dan (2016): New York City Taxi Trip Data (2010-2013). University of Illinois at Urbana-Champaign. https://doi.org/10.13012/J8PN93H8  
[2] Weiliang Zeng, Tomio Miwa, Takayuki Morikawa,
Prediction of vehicle CO2 emission and its application to eco-routing navigation, Transportation Research Part C: Emerging Technologies,
Volume 68, 2016, Pages 194-214. https://doi.org/10.1016/j.trc.2016.04.007  
[3] https://data.cityofnewyork.us/Transportation/VZV_Speed-Limits/7n5j-865y  
[4] https://github.com/2jungeuni/preprocessing-for-routing  
[5]