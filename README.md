# Installation

## Vorraussetzungen
- OpenCV, gegen das kompiliert werden kann
- Die Pakete libgazeboX-dev und gazeboX. X steht für die Versionsnummer. Getestet habe ich die Programme mit Version 7 und 9.


1. `git clone https://github.com/Sascha-Clausen/gazebo_bachelor.git ~/gazebo_bachelor`
2. Damit der Roboter heruntergeladen wird, gazebo mit folgendem Aufruf starten:
    ~~~~
    gazebo worlds/pioneer2dx.world --verbose
    ~~~~
3. In der Datei `~/gazebo_bachelor/models/custom_camera/model.sdf` den Pfad angeben, an den die Kamera-Matrix der montierten Kamera exportiert werden soll. Syntax:
    ~~~~    
    <plugin name="ProjectionMatrixPlugin" filename="libProjectionMatrixPlugin.so">  
      <filename>/home/sascha/gazebo/plugins/K.xml</filename>  
    </plugin>
    ~~~~
4. In der Datei `~/gazebo_bachelor/models/bachelor_robot/model.sdf` den Pfad angeben, an den die Translation und Rotation zwischen Lidar und Kamera exportiert werden soll. Syntax:
    ~~~
    <plugin name="ExtrinsicCalibrationPlugin" filename="libExtrinsicCalibrationPlugin.so">
      <filename>/home/sascha/gazebo/plugins/extrinsics.xml</filename>
    </plugin>
    ~~~
5. Model-Path setzen:
    ~~~
    export GAZEBO_MODEL_PATH=${GAZEBO_MODEL_PATH}:~/gazebo_bachelor/models
    ~~~
6. Kompilieren der Plugins:
    ~~~
    cd ~/gazebo_bachelor/gazebo_bachelor/plugins
    mkdir build
    cmake ..
    make
    ~~~
7. Plugin-Path setzen:
    ~~~
    export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/gazebo_bachelor/plugins/build
    ~~~
8. gazebo starten. Der erste Start dauert etwas länger, weil die restlichen noch benötigten Modelle runtergeladen werden:
    ~~~
    gazebo ~/gazebo_bachelor/worlds/acceleration.world --verbose
    ~~~


# Strecken

Die Strecken werden im xml-Format definiert und sind unter `worlds` abgelegt.

1. acceleration.world  
Die erste Strecke habe ich anhand des Regelwerks mit Model Populations erstellt (http://gazebosim.org/tutorials?tut=model_population).
2. skidpad.world  
Bei dieser komplexeren Strecke konnte ich auf Skripts zurückgreifen, die es im FSSIM-Repository gibt (https://github.com/AMZ-Driverless/fssim/tree/0cf64b5468bd91ecd35a5b15a39a863752cb9998/fssim_rqt_plugins/rqt_fssim_track_editor/src/rqt_fssim_track_editor).
Das Skript `skidpad.py` berechnet die Cone-Positionen. Ich habe das Skript etwas abgeändert und im Ordner `generate_tracks` abgelegt.
Das Skript `generate_track.py` liest diese Positionen aus und generiert daraus die Strecken-Datei.

`generate_track.py` sollte theoretisch auf für die Acceleration-Strecke funktionieren, aber das habe ich nicht getestet.
Außerdem ist hier das Programm beschrieben, dass die Welten erstellt:
https://github.com/AMZ-Driverless/fssim/blob/0cf64b5468bd91ecd35a5b15a39a863752cb9998/fssim_doc/configure.md#creating-a-new-track
Ich habe es mir aber nicht angesehen, also weiß ich nicht, ob wir das auch verwenden könnten.

# Modelle

Alle möglichen Parameter für die Sensoren sind auf http://sdformat.org/spec?ver=1.6&elem=sensor beschrieben.
Auf die Daten von Kamera und Lidar wird etwas weißes Rauschen draufgerechnet, dass sich aber auch abstellen lässt.
Die Parameter der Modelle können jeweils in der `model.sdf` angegeben werden.

### custom_camera
Einstellbare Parameter:
- horizontal_fov: 1.047
- image width: 1920
- image height: 1080
- near clipping: 0.1
- far clipping: 100
- update_rate: 30
- noise mean: 0.0
- noise stddev: 0.025

### lidar
Einstellbare Parameter:
- samples: 640  
Aus der Doku: The number of simulated rays to generate per complete laser sweep cycle.
- resolution: 1  
Aus der Doku: This number is multiplied by samples to determine the number of range data points returned. If resolution is less than one, range data is interpolated. If resolution is greater than one, range data is averaged.
- min_angle: -2.26889
- max_angle: 2.268899
- range min: 0.08
- range max: 10
- range resolution: 0.01
- update_rate: 30
- noise mean: 0.0
- noise stddev: 0.05

### bachelor_robot
Die Positionen und Rotationen der Kamera und des Lidar-Sensors können eingestellt werden. Dabei ist es nicht wichtig,
dass sie irgendwie verbunden sind, also können sie auch über dem Roboter schwebend montiert werden.

3. In der Datei `~/gazebo_bachelor/models/custom_camera/model.sdf` den Pfad angeben, an den die Kamera-Matrix der montierten Kamera exportiert werden soll. Syntax:
    ~~~~    
    <plugin name="ProjectionMatrixPlugin" filename="libProjectionMatrixPlugin.so">  
      <filename>/home/sascha/gazebo/plugins/K.xml</filename>  
    </plugin>
    ~~~~
4. In der Datei `~/gazebo_bachelor/ den Pfad angeben, an den die Translation und Rotation zwischen Lidar und Kamera exportiert werden soll. Syntax:


# Plugins
### ProjectionMatrixPlugin
SensorPlugin von custom_camera. Speichert die Kameramatrix der Kamera an den Speicherort, der in `models/custom_camera/model.sdf` angegeben ist. Syntax:
~~~~    
<plugin name="ProjectionMatrixPlugin" filename="libProjectionMatrixPlugin.so">  
  <filename>/home/sascha/gazebo/plugins/K.xml</filename>  
</plugin>
~~~~

### ExtrinsicCalibrationPlugin
ModelPlugin von bachelor_robot.  
Ziel: Speichert die extrinsische Kalibrierung von Lidar zu Kamera.  
Momentaner Stand: Liest die Positionen und Rotationen von Lidar und Kamera aus, aus denen man die extrinsische Kalibrierung berechnen kann.  
Speichert die Matrizen an den Speicherort, der in `models/custom_camera/model.sdf` angegeben ist. Syntax:
~~~
<plugin name="ExtrinsicCalibrationPlugin" filename="libExtrinsicCalibrationPlugin.so">
  <filename>/home/sascha/gazebo/plugins/extrinsics.xml</filename>
</plugin>
~~~

# Apps

### SendVelocityMessage
Ein einfaches Programm, mit dem der Roboter über die Kommandozeile gesteuert werden kann. Beispiel:
~~~
./SendVelocityMessage 0.5 -0.2
~~~
Die beiden Parameter sind die Vorwärtsgeschwindigkeit und die Drehgeschwindigkeit des Roboters.

### ObstacleFollowerApp
Ein einfacher Steuerungsalgorithmus, der über das Lidar das nächste Objekt sucht und darauf zu fährt.
Dabei werden per OpenCV die Lidar-Detektionen in das Kamera-Bild projiziert und das Bild angezeigt.

Aufgrund der Nebenläufigkeit von gazebo und der eigenen Programme muss der Zugriff auf die Daten der Kamera und des Lidar
über eine Semaphore geregelt werden. CamLidarHandler ist die Basis-Klasse, die die Nebenläufigkeit verwaltet und von der
alle Steuerungsklassen abgeleitet werden können.


Vor dem kompilieren sollte zunächst im Konstruktor von `CamLidarHandler.cc` die Pfade zur Kamera-Matrix und Extrinsik-Datei angeben werden. Dann:
~~~
cd /home/sascha/gazebo_bachelor/apps
mkdir build
cd build
cmake ..
make
~~~

### Klasse `CamLidarHandler`

- Kontrolliert Nebenläufigkeit über eine Semaphore
- Implementiert Grundfähigkeiten, sodass Implementierungen von CamLidarHandler abgeleitet werden können (Beispiel: ObstacleFollower)
- Abgeleitete Klassen müssen folgende Methoden implementieren:
    - `void MainLoop();`
    - `void OnScan(ConstLaserScanStampedPtr &msg);`
    - `void OnImage(ConstImageStampedPtr &msg);`

### Klasse `ObstacleFollower`
Ableitung von CamLidarHandler, das per Lidar das nächste Objekt sucht und darauf zu fährt.

### `ObstacleFollowerMain.cc`
Einfache main-Methode, die ein ObstacleFollower-Objekt erstellt und startet.
