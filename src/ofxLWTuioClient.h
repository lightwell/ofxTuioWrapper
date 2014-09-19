/*
 
 Re-purposed for ofxLWCommon by Richard Wong, 2013
 
 ===============================================================
 
 TUIO Client Wrapper for OpenFrameworks 
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 Based on the TUIO Demo by Martin Kaltenbrunner:
 
 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LW_TUIO_CLIENT
#define _LW_TUIO_CLIENT

#include "ofMain.h"
#include "TuioListener.h"
#include "TuioClient.h"
#include "TuioObject.h"
#include "TuioCursor.h"
#include "TuioPoint.h"

#include <list>
#include <queue>
#include <math.h>

using namespace TUIO;

// =======================================

class LWTuioCursor {
    
public:
    float xpos, ypos;
    float screenx, screeny;
    long session_id;
    bool busy;
    bool kill;
    
    LWTuioCursor(int _id, int _notUsed, float _xpos, float _ypos, float _screenx=0, float _screeny=0)
    {
        session_id = _id;
        xpos = _xpos;
        ypos = _ypos;
        screenx = _screenx;
        screeny = _screeny;
        busy = false;
        kill = false;
    }
    
    ~LWTuioCursor(){}
    
    float getX() { return xpos; }
    float getY() { return ypos; }
    float getScreenX() { return screenx; }
    float getScreenY() { return screeny; }
    long getSessionId() { return session_id; }
    long getSessionID() { return session_id; }
    
};

struct LWTuioEvent {
    LWTuioEvent(LWTuioCursor* _cursor, int _action)
    {
        cursor = _cursor;
        action = _action;
    }
    LWTuioCursor* cursor;
    int action;
};

// =======================================

typedef LWTuioCursor ofxTuioCursor;

// =======================================

class ofxLWTuioClient : public TuioListener{
	
public:
	ofxLWTuioClient();
	~ofxLWTuioClient() {
		disconnect();
	}
	
	void start(int _port);
	void disconnect();
	
    //GET THESE WORKING TO IMPLEMENT OBJECTS
    //======================================
	void addTuioObject(TuioObject * tobj){}
	void updateTuioObject(TuioObject * tobj){}
	void removeTuioObject(TuioObject * tobj){}
    //======================================
	
	void addTuioCursor(TuioCursor * tcur);
	void updateTuioCursor(TuioCursor * tcur);
	void removeTuioCursor(TuioCursor * tcur);
	
    // What is this.. refresh?
	void refresh(TuioTime frameTime){};
    
    void getMessage();
    
	void drawCursors();
	void drawObjects();
	
	void setVerbose(bool b);
    
    vector<LWTuioCursor*> cursors;
    queue<LWTuioEvent> events;
    
	ofEvent<LWTuioCursor> cursorAdded;
	ofEvent<LWTuioCursor> cursorRemoved;
	ofEvent<LWTuioCursor> cursorUpdated;
    
	//OF POCO EVENTS TO FORWARD TUIO EVENTS TO testApp or any other Class
	
    //======================================
	//ofEvent<TuioObject> objectAdded;
	//ofEvent<TuioObject> objectRemoved;
	//ofEvent<TuioObject> objectUpdated;
    //======================================
	
	TuioClient * client;
	
protected:
	bool bVerbose, bIsConnected;
};
#endif

