all:
	g++ display.cpp Boid.cpp Flocking.cpp Simulation.cpp dynamic.cpp PathFinder.cpp MapLoader.cpp -std=c++0x -D__LINUX_COMPILE -g -o flock-solve -lopenvdb -lHalf -ltbb -lGL -lglut -lGLU -lpthread -lX11

vdb_map_converter:
	g++ convertVDBtoMAP.cpp -std=c++0x -D__LINUX_COMPILE -lopenvdb -lHalf -ltbb -o vdb_map_converter

metaVDB:
	g++ insertMetaVDB.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o insertMetaVDB

sdfRead:
	g++ sdfRead.cpp -o test -lopenvdb -lHalf -ltbb
