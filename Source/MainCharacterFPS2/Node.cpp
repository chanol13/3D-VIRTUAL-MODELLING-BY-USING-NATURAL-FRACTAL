// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterFPS2.h"
#include "Node.h"
#include <stdlib.h>
#include <time.h>

Node::Node()
{
}

Node::~Node()
{
}

Node::TreeNode* Node::createTreeNextNode(TArray<float> startT, TArray<float> stopT, TArray<float> angleT, int type, int loopindex, float radiusstop)
{
	Node::TreeNode* newNode = new Node::TreeNode();
	newNode->start = startT;
	newNode->stop = stopT;
	newNode->angle = angleT;
	newNode->type = type;
	newNode->loopindex = loopindex;
	newNode->radiusstop = radiusstop;
	newNode->isempty = true;
	newNode->br = 0;
	return newNode;
}

Node::SurfaceplotData* Node::createSurfaceplotdata(TArray<float> start, float anglemax, float anglemin, TArray<float> angle, float maxheight, float minheight, float radiusstart, float radiusstop, float twist)
{

	Node::SurfaceplotData* newsurf = new Node::SurfaceplotData();
	float height = (maxheight - minheight)*randomprob() + minheight;
	TArray<float> newangle;

	//change random method
	//newangle.Add(angle[0] + (anglemax - anglemin)*randomprob() + anglemin);
	//newangle.Add(angle[1] + (anglemax - anglemin)*randomprob() + anglemin);
	//newangle.Add(0);
	float anglex;
	float angley;
	float anglez;
	float anglealpha;
	anglez = (anglemax - anglemin)*randomprob() + anglemin;
	//anglealpha = randomprob()*(360);
	anglealpha = twist;
	anglex = anglez*cos(anglealpha*PI / 180);
	angley = anglez*sin(anglealpha*PI / 180);
	newangle.Add(angle[0] + anglex);
	newangle.Add(angle[1] + angley);
	newangle.Add(0);
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f %f"), newangle[0], newangle[1], newangle[2] , (anglemax - anglemin)*randomprob());
	//newangle.Add(angle[2] + (anglemax - anglemin)*randomprob() + anglemin);
	//create cylinder
	int index1 = 0;
	TArray<float> theta;
	TArray<float> sintheta;
	while (index1 < 40) {
		theta.Add((index1 * 2 * PI) / 40);
		sintheta.Add(sin(theta[index1]));
		index1++;
	}
	sintheta[39] = 0;
	index1 = 0;
	TArray <Node::SurfData*> x;
	TArray <Node::SurfData*> y;
	TArray <Node::SurfData*> z;
	while (index1 < 10) {
		Node::SurfData* datax = new Node::SurfData();
		Node::SurfData* datay = new Node::SurfData();
		Node::SurfData* dataz = new Node::SurfData();
		int index2 = 0;
		float radius = radiusstart - (index1*((radiusstart - radiusstop) / 10));
		while (index2 < 40) {
			datax->data.Add(radius*cos(theta[index2]));
			datay->data.Add(radius*sintheta[index2]);
			dataz->data.Add(((height*index1) / 10));
			//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), radius*cos(theta[index2]), radius*sintheta[index2], (height*index1) / 10);
			index2++;
		}
		x.Add(datax);
		y.Add(datay);
		z.Add(dataz);
		index1++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), x[0]->data[0], y[0]->data[0], z[0]->data[0]);
	int log1 = 0;
	int log2 = 0;
	while (log1 < 10) {
		log2 = 0;
		while (log2 < 10) {
			//UE_LOG(LogTemp, Warning, TEXT("logsurf1 %f %f %f %d,%d"), x[log2]->data[log1], y[log2]->data[log1], z[log2]->data[log1], log2, log1);
			log2++;
		}
		log1++;
	}


	//rotate cylinder



	//x axis change
	index1 = 0;
	while (index1 < 40) {
		int index2 = 0;
		while (index2 < 10) {
			float newx = x[index2]->data[index1];
			float newy = y[index2]->data[index1];
			float newz = z[index2]->data[index1];
			//init x
			float myradx = sqrt(newy * newy + newz * newz);
			float myangle = (atan(newy / newz) * 180) / PI;
			//change x angle
			if (newz == 0) {
				myangle = (atan(newy / 0.00001) * 180) / PI;
			}
			myangle = myangle + newangle[0];
			//set new yz
			y[index2]->data[index1] = myradx*sin((myangle*PI) / 180);
			z[index2]->data[index1] = myradx*cos((myangle*PI) / 180);
			index2++;
			//UE_LOG(LogTemp, Warning, TEXT("rad : %f %f %f %f %f"), myradx, myangle, sin((myangle*PI) / 180), newy, newz);
		}
		index1++;
	}


	//UE_LOG(LogTemp, Warning, TEXT("s1 : %f %f %f"), x[0]->data[0], y[0]->data[0], z[0]->data[0]);
	//y axis change
	index1 = 0;
	while (index1 < 40) {
		int index2 = 0;
		while (index2 < 10) {
			float newx = x[index2]->data[index1];
			float newy = y[index2]->data[index1];
			float newz = z[index2]->data[index1];
			//init y
			float myrady = sqrt(newz * newz + newx * newx);
			float myangle = (atan(newx / newz) * 180) / PI;
			if (newz == 0) {
				myangle = (atan(newx / 0.00001) * 180) / PI;
			}
			//change x angle
			myangle = myangle + newangle[1];
			//set new yz
			x[index2]->data[index1] = myrady*sin((myangle*PI) / 180);
			z[index2]->data[index1] = myrady*cos((myangle*PI) / 180);
			index2++;
		}
		index1++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("s2 : %f %f %f"), x[0]->data[0], y[0]->data[0], z[0]->data[0]);

	/*
	//z axis change
	index1 = 0;
	while (index1 < 40) {
	int index2 = 0;
	while (index2 < 10) {
	float newx = x[index2]->data[index1];
	float newy = y[index2]->data[index1];
	float newz = z[index2]->data[index1];
	//init x
	float myradz = sqrt(newx * newx + newy * newy);
	float myangle = (atan(newx / newy) * 180) / PI;
	if (newy == 0) {
	myangle = 0;
	}
	//change x angle
	myangle = myangle + newangle[2];
	//set new yz
	x[index2]->data[index1] = myradz*sin(myangle);
	y[index2]->data[index1] = myradz*cos(myangle);
	index2++;
	}
	index1++;
	}
	*/
	//UE_LOG(LogTemp, Warning, TEXT("s3 : %f %f %f"), x[0]->data[0], y[0]->data[0], z[0]->data[0]);
	//set stop and height

	//newsurf->stop = last [10 this][40]
	//UE_LOG(LogTemp, Warning, TEXT("startP %f %f %f"), start[0], start[1], start[2]);
	int startP = 0;
	while (startP < x.Num()) {
		int startP2 = 0;
		while (startP2 < x[0]->data.Num()) {
			x[startP]->data[startP2] = x[startP]->data[startP2] + start[0];
			y[startP]->data[startP2] = y[startP]->data[startP2] + start[1];
			z[startP]->data[startP2] = z[startP]->data[startP2] + start[2];
			startP2++;
		}
		startP++;
	}
	newsurf->angle = newangle;
	newsurf->x = x;
	newsurf->y = y;
	newsurf->z = z;

	//x[9]->data[1-40]
	TArray<float> stop;
	float stopx = 0;
	float stopy = 0;
	float stopz = 0;
	int indexstop = 0;
	while (indexstop < 40) {
		stopx = stopx + x[9]->data[indexstop];
		stopy = stopy + y[9]->data[indexstop];
		stopz = stopz + z[9]->data[indexstop];
		indexstop++;
	}
	stopx = stopx / 40;
	stopy = stopy / 40;
	stopz = stopz / 40;
	stop.Add(stopx);
	stop.Add(stopy);
	stop.Add(stopz);
	newsurf->stop = stop;
	newsurf->alpha = anglealpha;
	//log all
	/*
	log1 = 0;

	while (log1 < 10) {
	log2 = 0;
	while (log2 < 10) {
	//UE_LOG(LogTemp, Warning, TEXT("logsurf2 %f %f %f %d,%d"), x[log2]->data[log1], y[log2]->data[log1], z[log2]->data[log1],log2,log1);
	log2++;
	}
	log1++;
	}
	*/
	return newsurf;
}

float Node::randomprob() {
	//random
	return FMath::FRandRange(0.0, 1.0);
}

Node::SurfaceplotData* Node::createJoint(TArray<float> px, TArray<float> py, TArray<float> pz, TArray<float> cx, TArray<float> cy, TArray<float> cz) {
	Node::SurfaceplotData* newsurf = new Node::SurfaceplotData();
	TArray<Node::SurfData*> x;
	TArray<Node::SurfData*> y;
	TArray<Node::SurfData*> z;
	Node::SurfData* px1 = new Node::SurfData;
	Node::SurfData* py1 = new Node::SurfData;
	Node::SurfData* pz1 = new Node::SurfData;
	Node::SurfData* cx1 = new Node::SurfData;
	Node::SurfData* cy1 = new Node::SurfData;
	Node::SurfData* cz1 = new Node::SurfData;
	px1->data = px;
	py1->data = py;
	pz1->data = pz;
	cx1->data = cx;
	cy1->data = cy;
	cz1->data = cz;
	x.Add(px1);
	x.Add(cx1);
	y.Add(py1);
	y.Add(cy1);
	z.Add(pz1);
	z.Add(cz1);
	newsurf->x = x;
	newsurf->y = y;
	newsurf->z = z;
	return newsurf;
}