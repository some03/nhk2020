# Branch  
master-->  
metric-->経路追従  
gain-->調節用   

## metric
* /src  
オドメトリの計算と配信  
odomからbase_linkへの変換  
モータのノード  
* /param  
move_baseのパラメータ  　
* /route  
手動で走らせた際のオドメトリ情報をyamlファイルに保存　　
* /script/main.py  　
smach まだ書いてない  
* /script/package/read_waypoint.py  
モジュール、routeにあるファイルを読み込んでmove_baseに目標値を送る　　
* /bmx/src　　
imuの読み取り
## tf tree
map->odom->base_link
