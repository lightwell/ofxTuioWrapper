/*
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

#include "ofxLWTuioClient.h"

ofxLWTuioClient::ofxLWTuioClient() {
    bIsConnected = false;	
	bVerbose = false;
}

void ofxLWTuioClient::start(int port){
	client = new TuioClient(port);
	client->addTuioListener(this);
	client->connect();
	
	if (!client->isConnected()) {
		cout<<"Could not connect TUIO Client"<<endl;
	} else {
	    bIsConnected = true;	
	}
	
}

void ofxLWTuioClient::disconnect(){
	if(bIsConnected) client->disconnect();
	bIsConnected = false;
}

void ofxLWTuioClient::setVerbose(bool b){
	bVerbose = b;
}

void ofxLWTuioClient::drawCursors(){
    std::list<TuioCursor*> cursorList = client->getTuioCursors();
	std::list<TuioCursor*>::iterator tit;
	client->lockCursorList();
	for (tit=cursorList.begin(); tit != cursorList.end(); tit++) {
		TuioCursor * cur = (*tit);
		//if(tcur!=0){
			//TuioCursor cur = *tcur;
//			glColor3f(0.0,0.0,0.0);
			ofEllipse(cur->getX()*ofGetWidth(), cur->getY()*ofGetHeight(), 10.0, 10.0);
			string str = "SessionId: "+ofToString((int)(cur->getSessionID()));
			ofDrawBitmapString(str, cur->getX()*ofGetWidth()-10.0, cur->getY()*ofGetHeight()+25.0);
			str = "CursorId: "+ofToString((int)(cur->getCursorID()));
			ofDrawBitmapString(str, cur->getX()*ofGetWidth()-10.0, cur->getY()*ofGetHeight()+40.0);
		//}
	}
	client->unlockCursorList();
}

void ofxLWTuioClient::drawObjects(){
    std::list<TuioObject*> objectList = client->getTuioObjects();
	list<TuioObject*>::iterator tobj;
	client->lockObjectList();
	for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
		TuioObject *obj = (*tobj);
		glColor3f(1.0,0.0,0.0);
		glPushMatrix();
		glTranslatef(obj->getX()*ofGetWidth(), obj->getY()*ofGetHeight(), 0.0);
		glRotatef(obj->getAngleDegrees(), 0.0, 0.0, 1.0);
		ofRect(-10.0, -10.0, 20.0, 20.0);
		glColor3f(1.0,1.0,1.0);
		ofLine(0, 0, 0, -10);
		glPopMatrix();
		string str = "SymbolId: "+ofToString((int)(obj->getSymbolID()));
		ofDrawBitmapString(str, obj->getX()*ofGetWidth()-10.0, obj->getY()*ofGetHeight()+25.0);
		str = "SessionId: "+ofToString((int)(obj->getSessionID()));
		ofDrawBitmapString(str, obj->getX()*ofGetWidth()-10.0, obj->getY()*ofGetHeight()+40.0);
	}
	client->unlockObjectList();
}

/* // GET THESE WORKING TO IMPLEMENT OBJECTS
 // =========================================
 
void ofxLWTuioClient::addTuioObject(TuioObject *tobj) {
	
	ofNotifyEvent(objectAdded, *tobj, this);
	
	if (bVerbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
	
}

void ofxLWTuioClient::updateTuioObject(TuioObject *tobj) {
	
	ofNotifyEvent(objectUpdated, *tobj, this);
	
	if (bVerbose) 	
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() 
		<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
	
}

void ofxLWTuioClient::removeTuioObject(TuioObject *tobj) {
	
	ofNotifyEvent(objectRemoved, *tobj, this);
	
	if (bVerbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}
 */

void ofxLWTuioClient::getMessage()
{
    // Iterate through event queue
    
    client->lockCursorList();
    while (!events.empty())
    {
        if(events.front().action == TUIO_ADDED) {
            
            ofNotifyEvent(cursorAdded, *events.front().cursor, this);
            
        } else if (events.front().action == TUIO_ACCELERATING) {
            
            ofNotifyEvent(cursorUpdated, *events.front().cursor, this);
            
        } else if(events.front().action == TUIO_REMOVED) {
            
            ofNotifyEvent(cursorRemoved, *events.front().cursor, this);
            events.front().cursor->kill = true;
            
        }
        
        events.pop();
	}
    client->unlockCursorList();
    
    // Remove unwanted cursors
    for (int i = cursors.size()-1; i >= 0; i--) {
        if (cursors[i]->kill) {
            delete cursors[i];
            cursors[i] = NULL;
            cursors.erase(cursors.begin() + i);
        }
    }
}

void ofxLWTuioClient::addTuioCursor(TuioCursor *tcur) {
    
    LWTuioCursor* newCur = new LWTuioCursor(tcur->getSessionID(), tcur->getSessionID(), tcur->getX(), tcur->getY(), tcur->getX() * ofGetWidth(), tcur->getY() * ofGetHeight());
    
    cursors.push_back( newCur );
    events.push( LWTuioEvent(newCur, TUIO_ADDED) );
    
}

void ofxLWTuioClient::updateTuioCursor(TuioCursor *tcur) {
    
    for (int i = 0; i < cursors.size(); i++) {
        if (tcur->getSessionID() == cursors[i]->getSessionID()) {
            cursors[i]->xpos = tcur->getX();
            cursors[i]->ypos = tcur->getY();
            cursors[i]->screenx = tcur->getX() * ofGetWidth();
            cursors[i]->screeny = tcur->getY() * ofGetHeight();
            events.push( LWTuioEvent(cursors[i], TUIO_ACCELERATING) );
            break;
        }
    }
    
}

void ofxLWTuioClient::removeTuioCursor(TuioCursor *tcur) {
    
    for (int i = 0; i < cursors.size(); i++) {
        if (tcur->getSessionID() == cursors[i]->getSessionID()) {
            cursors[i]->xpos = tcur->getX();
            cursors[i]->ypos = tcur->getY();
            cursors[i]->xpos = tcur->getX() * ofGetWidth();
            cursors[i]->ypos = tcur->getY() * ofGetHeight();
            events.push( LWTuioEvent(cursors[i], TUIO_REMOVED) );
            break;
        }
    }
    
}



