// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class MAINCHARACTERFPS2_API Node
{
public:
	Node();
	~Node();
	struct SurfData {
		TArray<float> data;
	};
	struct SurfaceplotData {
		TArray <SurfData*> x;
		TArray <SurfData*> y;
		TArray <SurfData*> z;
		TArray <float> stop;
		TArray <float> angle;
		float alpha = 0;
	};
	struct TreeNode {
		TArray <TreeNode*> TreeNodeArray;
		TArray <float> start;
		TArray <float> stop;
		TArray <float> angle;
		SurfaceplotData* data;
		float radiusstop;
		int type;
		int loopindex;
		bool isempty;
		float twist;
		int br;
	};
	float randomprob();
	TreeNode* createTreeNextNode(TArray<float> startT, TArray<float> stopT, TArray<float> angleT, int type, int loopindex, float radiusstop);
	SurfaceplotData* createSurfaceplotdata(TArray<float> start, float anglemax, float anglemin, TArray<float> angle, float maxheight, float minheight, float radiusstart, float radiusstop, float twist);
	SurfaceplotData* createJoint(TArray<float> px, TArray<float> py, TArray<float> pz, TArray<float> cx, TArray<float> cy, TArray<float> cz);
};
