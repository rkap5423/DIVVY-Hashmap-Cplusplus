/*main.cpp*/

//
// Raj Kapadia
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// DIVVY Bike Project
// 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "hash.h"
#include "hashmap.h"
#include "util.h"
#include <algorithm>

using namespace std;

//Station struct
struct Stations
{
    string id; 
    string abbrev;
    string fullname;
    string latitude;
    string longitude;
    string cap;
    string onlinedate;
    Stations(){ //initialize to default values
        id = "";
        abbrev = "";
        fullname = "";
        latitude = "";
        longitude = "";
        cap = "";
        onlinedate = "";
    }
};

//Trips struct
struct Trips
{
    string tripid;
    string starttime;
    string stoptime;
    string bikeid;
    string duration;
    string from;
    string to;
    string identifies;
    string birthyear;
    string bikeNum;
    
    Trips() //initialize to default values
    {
      tripid = "";
      starttime = "";
      stoptime = "";
      bikeid = "";
      duration = "";
      from = "";
      to = "";
      identifies = "";
      birthyear = "";  
      bikeNum="1";
    }
};

//helper struct for commands 5 and 6
struct nearbyStruct{
    string id;
    double distance;
    nearbyStruct(){ //initialize to default values
        id="";
        distance=0;
    }
};

//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}

//
// inputDataStat
// 
// Given a filename denoting station information,
// inputs that data into the given hash table(s).
// 
bool inputDataStat(string filename, int& count,hashmap<string, Stations>& stationsById, hashmap<string, Stations>& stationsByAbbrev)
{
	ifstream  infile(filename);
	
	cout << "Reading " << filename << endl;
	
	if (!infile.good()) //checks if file can be opened
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
	count = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
		string ID, Abbrev, Fullname, Latitude, Longitude, capacity, onlineDate;
		
		getline(s, ID, ',');  // get station ID
		getline(s, Abbrev, ',');   // get abbreviation
		getline(s, Fullname, ','); //get full station name
		getline(s, Latitude, ','); //get station latitude
        getline(s, Longitude, ',');//get station longitude
        getline(s, capacity, ','); //get station capacity
        getline(s, onlineDate, ','); //get the date the station came online
		

        Stations stat; //declare temporary stations variable
        //copy over stations values into the variable
        stat.id=ID;
        stat.abbrev=Abbrev;
        stat.fullname=Fullname;
        stat.latitude=Latitude;
        stat.longitude=Longitude;
        stat.cap=capacity;
        stat.onlinedate=onlineDate;
       
        //insert the variable into the StationID hashmap    
        stationsById.insert(ID,stat,HashStatID);
        //insert the variable into the StationAbbrev hashmap
		stationsByAbbrev.insert(Abbrev, stat, HashStatAbbrev);
		count++; //keep track of number of stations inserted
	}
		
	return true;  // we have data to be processed:
}



//
// inputDataTrip
// 
// Given a filename denoting trip information,
// inputs that data into the given hash table(s).
// 
bool inputDataTrip(string filename, int& countT, int& countB, hashmap<string, Trips>& tripsById, hashmap<string, string>& bikesById)
{
	ifstream  infile(filename);
	
	cout << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
	countT = 0;
    countB = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
		string tID, start, stop, bID, dur, From, To, Identifies, birth;
		
		getline(s, tID, ',');  // get trip ID
		getline(s, start, ',');   // get trip start time
		getline(s, stop, ','); //get trip end time
		getline(s, bID, ','); //get ID of bike used
        getline(s, dur, ','); //get duration of trip
        getline(s, From, ',');//get start location
        getline(s, To, ',');//get end location
        getline(s, Identifies, ','); //get gender of rider
        getline(s, birth, ',');//get birthdate of rider
		

        Trips trip;//declare temporary trips variable
        //store data into variable
        trip.tripid=tID;
        trip.starttime=start;
        trip.stoptime=stop;
        trip.bikeid=bID;
        trip.duration=dur;
        trip.from=From;
        trip.to=To;
        trip.identifies=Identifies;
        trip.birthyear=birth;
        
        string num="1"; //initialize variable to search for how many trips a bike has been in
        tripsById.insert(tID,trip,HashTripID); //insert into tripID hashmap
        if(bikesById.search(bID,num,HashBikeID)==false){
            countB++; //new bike used, increment counter
        }
        else{
            num=to_string(stoi(num)+1);
        }
		bikesById.insert(bID, num, HashBikeID); //insert into bikeId hashmap
		countT++; //keep track of total number of trips
	}
	
	return true;  // we have data to be processed:
}

//helper function to return a vector of all stations within a given distance of a given station
vector<nearbyStruct> valid(vector<string> list, hashmap<string, Stations>& stationsById, string lat1, string long1, string distance){
    vector<nearbyStruct> tempV;
    Stations cur;
    for (size_t i=0;i<list.size();i++){ //checks all station IDs
            stationsById.search(list.at(i),cur,HashStatID); //retrieves station information and stores into cur
            //checks if the current station in the loop is within the valid "radius"
            if (distBetween2Points(stod(lat1),stod(long1),stod(cur.latitude),stod(cur.longitude))<=stod(distance)){
                nearbyStruct temp;
                //if it is, store the information into temp struct
                temp.id=list.at(i);
                temp.distance=distBetween2Points(stod(lat1),stod(long1),stod(cur.latitude),stod(cur.longitude));
                tempV.push_back(temp); //push struct into vector
            }
        }
    return tempV;
}

//helper function to sort vector, returns boolean value
bool compare(const nearbyStruct &a, const nearbyStruct &b)
{
    return stod(a.id) < stod(b.id); //sorts IDs
}


int main()
{
	cout<< "** DIVVY analysis program **" << endl << endl;

    //declare hashtables
	hashmap<string, Stations> stationsById(10000); // 10K
    hashmap<string, Stations> stationsByAbbrev(10000); // 10K
    hashmap<string, Trips> tripsById(2500000); // 2.5M
    hashmap<string, string> bikesById(50000); // 50K
 
	string stationsFilename = "stations.csv";
    string tripsFilename = "trips.csv";
    string filenameStat;
    string filenameTrip;
    int count;
    int countT;
    int countB;
    
    cout << "Enter stations file> ";
    getline(cin, filenameStat);// user can type a filename, or just press ENTER    
    if (filenameStat != "")// user entered a filename, use it
        stationsFilename = filenameStat;
   
    
    cout << "Enter trips file> ";
    getline(cin, filenameTrip);// user can type a filename, or just press ENTER
    cout<<endl;
    if (filenameTrip != ""){// user entered a filename, use it
        tripsFilename = filenameTrip;
    }
	bool successStat = inputDataStat(stationsFilename, count,stationsById,stationsByAbbrev); //input station data
	bool successTrip=inputDataTrip(tripsFilename,countT,countB,tripsById,bikesById); //input trip/bike data
    cout<<endl;
    
	//
	// did we input anything?
	// 
	if (!successStat || !successTrip) //check if inputting was successful 
	{
		cout << "No data, file not found?" << endl;
		return 0;
	}
    cout << "# of stations: " << count << endl;
    cout << "# of trips: " << countT << endl;
    cout << "# of bikes: " << countB << endl;

    
    string command; 
	cout << endl;
	cout << "Please enter a command, help, or #>";
	getline(cin,command);

	//
	// main loop and program starts:
	//
	while (command != "#")
	{
    Stations st;
    Trips tr;
    if (command=="help"){ //output list of all commands
         cout << "Available commands: " << endl;
         cout << " Enter a station id (e.g. 341)" << endl;
         cout << " Enter a station abbreviation (e.g. Alder)" << endl;
         cout << " Enter a trip id (e.g. Tr10426561)" << endl;
         cout << " Enter a bike id (e.g. B5218)" << endl;
         cout << " Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
         cout << " Similar trips (e.g. similar Tr10424639 0.3)" << endl;
    }
    else if (isNumeric(command)){ //execute command 1, station ID search
        bool found=stationsById.search(command,st,HashStatID);
        if (found==true){
            cout<<" Station: "<<endl;
            cout<<" ID: "<<st.id<<endl;
            cout<<" Abbrev: "<<st.abbrev<<endl;
            cout<<" Fullname: "<<st.fullname<<endl;
            cout<<" Location: ("<<stod(st.latitude)<<", "<<stod(st.longitude)<<")"<<endl;
            cout<<" Capacity: "<<st.cap<<endl;
            cout<<" Online date: "<<st.onlinedate<<endl;
        }
        else
            cout<<" Station not found"<<endl;
    }
        //execute command 3, Trip ID search
    else if (command.length()>2 && command.substr(0,2)=="Tr" && isNumeric(command.substr(2))){
        bool found=tripsById.search(command,tr,HashTripID);
        if (found==true){
            //convert duration into hours, minutes, seconds
            int total=stoi(tr.duration);
            int min=total/60;
            int sec=total%60;
            int hour=min/60;
            min=min%60;
            cout<<" Trip: "<<endl;
            //output Trip information
            cout<<" ID: "<<tr.tripid<<endl;
            cout<<" Starttime: "<<tr.starttime<<endl;
            cout<<" Bikeid: "<<tr.bikeid<<endl;
            cout<<" Duration: ";
            if (hour>0)
                cout<<hour<<"hours, ";
            if (min>0)
                cout<<min<<" minutes, ";
            cout<<sec<<" seconds";
            cout<<endl;
            
            //special case, need stations variables to retrieve and output From/To station names
            Stations temp;
            stationsById.search(tr.from,temp,HashStatID);
            Stations temp2;
            stationsById.search(tr.to,temp2,HashStatID);
            cout<<" From station: "<<temp.abbrev<<" ("<<tr.from<<")"<<endl;
            cout<<" To station: "<<temp2.abbrev<<" ("<<temp2.id<<")"<<endl;
            
            //output remaining Trip info
            cout<<" Rider identifies as: "<<tr.identifies<<endl;
            cout<<" Birthyear: "<<tr.birthyear<<endl;
        }
        else
            cout<<" Trip not found"<<endl;
    }
    //command 4, lookup Bike usage    
    else if (command.length()>1 && command.substr(0,1)=="B" && isNumeric(command.substr(1))){
        string temp;
        bool found=bikesById.search(command,temp,HashBikeID); //search in bikes hashmap
        if (found==true){
            cout<<" Bike: "<<endl;
            cout<<" ID: "<<command<<endl;
            cout<<" Usage: "<<temp<<endl;
        }
        else{
            cout<<" Bike not found"<<endl;
        }
        
    }
    //command 5, find nearby stations    
    else if (command.substr(0,6)=="nearby"){
        vector<string> idList=stationsById.returnList(); //get a vector of all station IDs
        vector<nearbyStruct> validStations; //declare vector of custom structs, will hold information of all valid nearby stations
        command.erase(0,7);
        string lat1;
        string long1;
        string distance;
        stringstream stream(command);
        //parse and store lat/long/distance info
        getline(stream, lat1, ' ');
        getline(stream, long1, ' ');
        getline(stream, distance, ' ');
        
        
        validStations=valid(idList,stationsById,lat1,long1,distance); //store all stations within inputted distance in validStations vector
        cout<<" Stations within "<<distance<<" miles of ("<<lat1<<", "<<long1<<")"<<endl;
        if (validStations.size()==0){
            cout<<" none found"<<endl;
        }
        else{
            //run sorting function/algorithm, sorts by distance then ID
            sort(validStations.begin(), validStations.end(), [](nearbyStruct a, nearbyStruct b) {
                if (a.distance != b.distance) 
                    return a.distance < b.distance;
                else
                    return a.id < b.id;
            });
            //outputs sorted vector
            for (size_t k=0;k<validStations.size();k++){
                  cout<<" station "<<validStations.at(k).id<<": "<<validStations.at(k).distance<<" miles"<<endl;
            }
        }
    }
    //command 6, similar command
    else if (command.substr(0,7)=="similar"){
        command.erase(0,8);
        string inputT;
        string distance;
        stringstream stream(command);
        //parse and store tripid and distance 
        getline(stream, inputT, ' ');
        getline(stream, distance, ' ');
        
        vector<string> idList=stationsById.returnList(); //get vector of all station IDs
        vector<string> tripList=tripsById.returnList(); //get vector of all trip IDs
        Trips temp;
        bool found=tripsById.search(inputT,temp,HashTripID); //search trip hashmap for inputted Trip ID
        cout<<" Trips that follow a similar path (+/-"<<distance<<" miles) as "<<inputT<<endl;
        if (found==false){
            cout<<" no such trip"<<endl;
        }
        else{
            Stations fromStat;
            stationsById.search(temp.from,fromStat,HashStatID); //search stations hashmap for From station
            vector<nearbyStruct> validFromStations;
            validFromStations=valid(idList,stationsById,fromStat.latitude,fromStat.longitude,distance); //fill vector with all stations nearby From station
            
            Stations toStat;
            stationsById.search(temp.to,toStat,HashStatID); //search stations hashmap for To station
            vector<nearbyStruct> validToStations;
            validToStations=valid(idList,stationsById,toStat.latitude,toStat.longitude,distance); //fill vector with all stations nearby To station
            
            int tripCounter=0;
            Stations fromTemp;
            Stations toTemp;
            for (size_t z=0;z<tripList.size();z++){ //search every existing trip for one that's similar to inputted trip 
                tripsById.search(tripList.at(z),temp,HashTripID); //search trip hashmap for current trip in loop
                stationsById.search(temp.from,fromTemp,HashStatID); //search stations hashmap for From station
                stationsById.search(temp.to,toTemp,HashStatID); //search stations hashmap for To station
                
                //store how far From station is from inputted station
                double fromD=distBetween2Points(stod(fromTemp.latitude),stod(fromTemp.longitude),stod(fromStat.latitude),stod(fromStat.longitude));
                //store how far To station is from inputted station
                double toD=distBetween2Points(stod(toTemp.latitude),stod(toTemp.longitude),stod(toStat.latitude),stod(toStat.longitude));
                if (fromD<=stod(distance) && toD<=stod(distance)){ //if distances of both stations in the trip are valid, then the trip is similar and
                    tripCounter++;                             //counter is incremented
                }
            }  
            //sort both nearby vectors
            sort(validFromStations.begin(), validFromStations.end(), compare);
            sort(validToStations.begin(), validToStations.end(), compare);
            cout<<" nearby starting points: ";
            //output From stations
            for (size_t t=0;t<validFromStations.size();t++){
                cout<<validFromStations.at(t).id<<" ";
            }
            cout<<endl;
            cout<<" nearby ending points: ";
            //output To stations
            for (size_t y=0;y<validToStations.size();y++){
                cout<<validToStations.at(y).id<<" ";
            }
            cout<<endl;
            //output number of similar trips
            cout<<" trip count: "<<tripCounter<<endl;
        }
        
    }
    //command 2, lookup by station abbreviation
    else{
        bool found=stationsByAbbrev.search(command,st,HashStatAbbrev);
        if (found==true){
            cout<<" Station: "<<endl;
            cout<<" ID: "<<st.id<<endl;
            cout<<" Abbrev: "<<st.abbrev<<endl;
            cout<<" Fullname: "<<st.fullname<<endl;
            cout<<" Location: ("<<stod(st.latitude)<<", "<<stod(st.longitude)<<")"<<endl;
            cout<<" Capacity: "<<st.cap<<endl;
            cout<<" Online date: "<<st.onlinedate<<endl;
        }
        else{
            cout<<" Station not found"<<endl;
        }
    }
      cout << endl;
	  cout << "Please enter a command, help, or #>";
	  getline(cin,command);
	}
	
	
	return 0;
}
