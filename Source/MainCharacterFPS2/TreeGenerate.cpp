// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterFPS2.h"
#include "TreeGenerate.h"


// Sets default values
ATreeGenerate::ATreeGenerate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void ATreeGenerate::BeginPlay()
{
	Super::BeginPlay();
	inputconverter();
	CreateTreewithjoint();
	branchplot();
	createleaves();
}

void ATreeGenerate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreeGenerate::inputconverter() {
	int index = 0;
	while (index < branch.Num()) {
		BranchFrom.Add(branch[index].From);
		BranchPerLoop.Add(branch[index].Child);
		BranchTime.Add(branch[index].Times);
		Phyllotaxis.Add(branch[index].Phyllotaxis);
		BranchProb.Add(branch[index].Probability);
		BranchHeightMax.Add(branch[index].HeightMaximum);
		BranchHeightMin.Add(branch[index].HeightMinimum);
		BranchRadiusStart.Add(branch[index].Radius);
		BranchRadiusdecrease.Add(branch[index].RadiusDecrease);
		BranchAngleMax.Add(branch[index].AngleMax);
		BranchAngleMin.Add(branch[index].AngleMin);
		index++;
	}
	index = 0;
	while (index < leaves.Num()) {
		MapBranchtoLeavesbyType.Add(leaves[index].From);
		LeavesMaterial.Add(leaves[index].LeafMaterial);
		LeavesHeight.Add(leaves[index].Height);
		LeavesWidth.Add(leaves[index].Width);
		LeavesRotate.Add(leaves[index].Rotate);
		LeavesRotateRandom.Add(leaves[index].RotateRandom);
		index++;
	}
}

void ATreeGenerate::CreateTreewithjoint() {
	root = new Node();
	TArray<float> root_init;
	root_init.Add(0);
	root_init.Add(0);
	root_init.Add(0);
	Node::TreeNode* rootTree = root->createTreeNextNode(root_init, root_init, root_init, -1, 0, BranchRadiusStart[0]);
	Node::SurfaceplotData* surfaceroot = root->createSurfaceplotdata(root_init, 0, 0, root_init, 0, 0, BranchRadiusStart[0], BranchRadiusStart[0], 0);
	rootTree->data = surfaceroot;
	TArray<Node::TreeNode*> allTreeindex;
	rootTree->twist = 0;
	allTreeindex.Add(rootTree);
	int branchType = 0;
	while (branchType < BranchPerLoop.Num()) {
		int allTree_index = 0;
		while (allTree_index < allTreeindex.Num()) {
			bool check = false;
			if (allTreeindex[allTree_index]->type == BranchFrom[branchType]) {
				int btime = 0;
				while (btime < BranchPerLoop[branchType]) {
					if (randomtime(BranchProb[branchType])) {
						float twist = 0;
						twist = allTreeindex[allTree_index]->twist + Phyllotaxis[branchType];
						int br = allTreeindex[allTree_index]->br;
						br = br + 1;
						allTreeindex[allTree_index]->br = br;
						float twistangle;
						if (allTreeindex[allTree_index]->type < 0) {
							twistangle = -1;
						}
						else {
							twistangle = 360 / ((float)BranchPerLoop[branchType]);
						}
						twist = twist + btime*twistangle;
						TArray<float> startC = allTreeindex[allTree_index]->stop;
						float anglemax = BranchAngleMax[branchType];
						float anglemin = BranchAngleMin[branchType];
						TArray<float> angleS = allTreeindex[allTree_index]->angle;
						float maxheight = BranchHeightMax[branchType];
						float minheight = BranchHeightMin[branchType];
						float radiusstart = BranchRadiusStart[branchType];
						float radiusstop = BranchRadiusStart[branchType] - BranchRadiusdecrease[branchType];
						if (radiusstop < 0) {
							radiusstop = 0;
						}
						Node::SurfaceplotData* surfdt = root->createSurfaceplotdata(startC, anglemax, anglemin, angleS, maxheight, minheight, radiusstart, radiusstop, twist);//twist*(randomprob()+9.5)/10);
						TArray<float> stopC = surfdt->stop;
						TArray<float> angleC = surfdt->angle;
						int type = branchType;
						int loopindex = 0;
						Node::TreeNode* newtree = root->createTreeNextNode(startC, stopC, angleC, type, loopindex, radiusstop);
						newtree->data = surfdt;
						newtree->twist = randomprob() * 360;
						allTreeindex.Add(newtree);
						allTreeindex[allTree_index]->TreeNodeArray.Add(newtree);
					}
					btime++;
				}
			}
			else if (allTreeindex[allTree_index]->type == branchType && allTreeindex[allTree_index]->loopindex + 1 < BranchTime[branchType]) {
				int btime = 0;
				while (btime < BranchPerLoop[branchType]) {
					if (randomtime(BranchProb[branchType])) {
						float twist = 0;
						twist = randomprob() * 360;
						TArray<float> startC = allTreeindex[allTree_index]->stop;
						float anglemax = BranchAngleMax[branchType];
						float anglemin = BranchAngleMin[branchType];
						TArray<float> angleS = allTreeindex[allTree_index]->angle;
						float maxheight = BranchHeightMax[branchType];
						float minheight = BranchHeightMin[branchType];
						float radiusstart = allTreeindex[allTree_index]->radiusstop;
						float radiusstop = BranchRadiusStart[branchType] - BranchRadiusdecrease[branchType];
						if (radiusstop < 0) {
							radiusstop = 0;
						}
						Node::SurfaceplotData* surfdt = root->createSurfaceplotdata(startC, anglemax, anglemin, angleS, maxheight, minheight, radiusstart, radiusstop, twist);
						TArray<float> stopC = surfdt->stop;
						TArray<float> angleC = surfdt->angle;
						int type = branchType;
						int loopindex = allTreeindex[allTree_index]->loopindex + 1;
						Node::TreeNode* newtree = root->createTreeNextNode(startC, stopC, angleC, type, loopindex, radiusstop);
						newtree->data = surfdt;
						twist = allTreeindex[allTree_index]->twist + Phyllotaxis[branchType];
						newtree->twist = twist;
						allTreeindex.Add(newtree);
						allTreeindex[allTree_index]->TreeNodeArray.Add(newtree);
					}
					btime++;
				}
			}
			allTree_index++;
		}
		branchType++;
	}
	branchindex = allTreeindex;
	int treeindex = 0;
	while (treeindex < branchindex.Num()) {
		TArray<Node::TreeNode*> child = branchindex[treeindex]->TreeNodeArray;
		TArray<float> px = branchindex[treeindex]->data->x[9]->data;
		TArray<float> py = branchindex[treeindex]->data->y[9]->data;
		TArray<float> pz = branchindex[treeindex]->data->z[9]->data;
		int treechild = 0;

		while (treechild < child.Num()) {
			if (checkleaf(child[treechild]->type)) {
				TArray<float> cx = child[treechild]->data->x[0]->data;
				TArray<float> cy = child[treechild]->data->y[0]->data;
				TArray<float> cz = child[treechild]->data->z[0]->data;
				Node::SurfaceplotData* newsurf = root->createJoint(px, py, pz, cx, cy, cz);
				TArray<float> token;
				token.Add(0);
				token.Add(0);
				token.Add(0);
				Node::TreeNode* newjoint = root->createTreeNextNode(token, token, token, -2, 0, 0);
				newjoint->data = newsurf;
				allTreeindex.Add(newjoint);
			}
			treechild++;
		}
		treeindex++;
	}
	branchindex = allTreeindex;
}


void ATreeGenerate::CreateTree() {
	root = new Node();
	TArray<float> root_init;
	root_init.Add(0);
	root_init.Add(0);
	root_init.Add(0);
	Node::TreeNode* rootTree = root->createTreeNextNode(root_init, root_init, root_init, 0, -1, BranchRadiusStart[0]);
	Node::SurfaceplotData* surfaceroot = root->createSurfaceplotdata(root_init, 0, 0, root_init, 0, 0, BranchRadiusStart[0], BranchRadiusStart[0], 0);
	rootTree->data = surfaceroot;
	TArray<Node::TreeNode*> allTreeindex;
	allTreeindex.Add(rootTree);
	int branchType = 0;
	int indexTree = 1;
	if (allTreeindex.Num() == 1) {
		indexTree = 0;
	}
	while (branchType < BranchPerLoop.Num()) {
		indexTree = 0;
		while (indexTree < allTreeindex.Num()) {
			int indexbranch = 0;
			TArray<Node::TreeNode*> nodearray;
			bool nodeempty = true;
			while (indexbranch < BranchPerLoop[branchType]) {
				bool checkSametypeLastNode = false;
				if (allTreeindex[indexTree]->type == branchType && allTreeindex[indexTree]->loopindex == BranchPerLoop[allTreeindex[indexTree]->type] - 1) {
					checkSametypeLastNode = true;
				}
				if (!checkSametypeLastNode || allTreeindex[indexTree]->type == branchType - 1) {
					if (randomtime(BranchProb[branchType])) {
						TArray<float> start_new;
						start_new = allTreeindex[indexTree]->stop;
						float radiusstop_new = allTreeindex[indexTree]->radiusstop - (BranchRadiusdecrease[branchType] * randomprob());
						Node::SurfaceplotData* newsurface = root->createSurfaceplotdata(start_new, BranchAngleMax[branchType], BranchAngleMin[branchType], allTreeindex[indexTree]->angle, BranchHeightMax[branchType], BranchHeightMin[branchType], allTreeindex[indexTree]->radiusstop, radiusstop_new, 0);
						TArray<float> stop_new;
						stop_new = newsurface->stop;
						TArray<float> angle_new;
						angle_new = newsurface->angle;
						int type_new;
						int loopindex_new;
						type_new = allTreeindex[indexTree]->type;
						loopindex_new = allTreeindex[indexTree]->loopindex + 1;
						if (branchType != type_new) {
							type_new = branchType;
							loopindex_new = 0;
						}
						Node::TreeNode* newTree = root->createTreeNextNode(start_new, stop_new, angle_new, type_new, loopindex_new, radiusstop_new);
						newTree->data = newsurface;
						if (loopindex_new < BranchTime[branchType]) {
							nodearray.Add(newTree);
							allTreeindex.Add(newTree);
							nodeempty = false;
						}
					}
				}
				indexbranch++;
			}
			if (!nodeempty) {
				if (allTreeindex[indexTree]->isempty) {
					allTreeindex[indexTree]->TreeNodeArray = nodearray;
					allTreeindex[indexTree]->isempty = false;
				}
				else {
					int token_index = 0;
					while (token_index < nodearray.Num()) {
						allTreeindex[indexTree]->TreeNodeArray.Add(nodearray[token_index]);
						token_index++;
					}
				}
			}
			indexTree++;
		}
		branchType++;
	}
	int debugindex = 1;
	while (debugindex < allTreeindex.Num()) {
		Node::SurfaceplotData* dt = allTreeindex[debugindex]->data;
		TArray <Node::SurfData*> myx = dt->x;
		TArray <Node::SurfData*> myy = dt->y;
		TArray <Node::SurfData*> myz = dt->z;
		int index_debugArray1 = 0;
		while (index_debugArray1 < myx.Num()) {
			Node::SurfData* mySx = myx[index_debugArray1];
			Node::SurfData* mySy = myy[index_debugArray1];
			Node::SurfData* mySz = myz[index_debugArray1];
			Node::SurfData* test = mySx;
			test = mySy;
			test = mySz;
			index_debugArray1++;
		}
		debugindex++;
	}
	branchindex = allTreeindex;
}

float ATreeGenerate::randomprob() {
	return  FMath::FRandRange(0.0, 1.0);
}

void ATreeGenerate::branchplot() {
	int branchindex_i = 0;
	TArray<FVector> vertices;
	TArray<int> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;
	while (branchindex_i < branchindex.Num()) {
		if (checkleaf(branchindex[branchindex_i]->type)) {
			Node::SurfaceplotData* dt = branchindex[branchindex_i]->data;
			TArray <Node::SurfData*> x = dt->x;
			TArray <Node::SurfData*> y = dt->y;
			TArray <Node::SurfData*> z = dt->z;
			int index = 0;
			Node::SurfData* a = x[0];
			float xaxis = 1 / (float)x.Num();
			float yaxis = 1 / (float)a->data.Num();
			while (index + 1 < x.Num()) {
				Node::SurfData* sx = x[index];
				Node::SurfData* sy = y[index];
				Node::SurfData* sz = z[index];
				Node::SurfData* sxt = x[index + 1];
				Node::SurfData* syt = y[index + 1];
				Node::SurfData* szt = z[index + 1];
				int index1 = 0;
				while (index1 < (sx->data).Num()) {
					if (index1 + 1 == (sx->data).Num()) {
						float pointx1 = sx->data[index1];
						float pointx2 = sx->data[0];
						float pointx3 = sxt->data[index1];
						float pointx4 = sxt->data[0];
						float pointy1 = sy->data[index1];
						float pointy2 = sy->data[0];
						float pointy3 = syt->data[index1];
						float pointy4 = syt->data[0];
						float pointz1 = sz->data[index1];
						float pointz2 = sz->data[0];
						float pointz3 = szt->data[index1];
						float pointz4 = szt->data[0];
						FVector v1 = FVector(pointx1, pointy1, pointz1);
						FVector v2 = FVector(pointx2, pointy2, pointz2);
						FVector v3 = FVector(pointx3, pointy3, pointz3);
						FVector v4 = FVector(pointx4, pointy4, pointz4);
						vertices.Add(v1);
						vertices.Add(v2);
						vertices.Add(v3);
						vertices.Add(v4);
						int tri = vertices.Num();
						Triangles.Add(tri - 4);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 4);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 4);
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						UV0.Add(FVector2D(yaxis*(index1 + 0), xaxis*(index + 0)));
						UV0.Add(FVector2D(yaxis*(index1 + 1), xaxis*(index + 0)));
						UV0.Add(FVector2D(yaxis*(index1 + 0), xaxis*(index + 1)));
						UV0.Add(FVector2D(yaxis*(index1 + 1), xaxis*(index + 1)));
					}
					else {
						float pointx1 = sx->data[index1];
						float pointx2 = sx->data[index1 + 1];
						float pointx3 = sxt->data[index1];
						float pointx4 = sxt->data[index1 + 1];
						float pointy1 = sy->data[index1];
						float pointy2 = sy->data[index1 + 1];
						float pointy3 = syt->data[index1];
						float pointy4 = syt->data[index1 + 1];
						float pointz1 = sz->data[index1];
						float pointz2 = sz->data[index1 + 1];
						float pointz3 = szt->data[index1];
						float pointz4 = szt->data[index1 + 1];
						FVector v1 = FVector(pointx1, pointy1, pointz1);
						FVector v2 = FVector(pointx2, pointy2, pointz2);
						FVector v3 = FVector(pointx3, pointy3, pointz3);
						FVector v4 = FVector(pointx4, pointy4, pointz4);
						vertices.Add(v1);
						vertices.Add(v2);
						vertices.Add(v3);
						vertices.Add(v4);
						int tri = vertices.Num();
						Triangles.Add(tri - 4);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 4);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 1);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 3);
						Triangles.Add(tri - 2);
						Triangles.Add(tri - 4);
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						normals.Add(FVector(1, 0, 0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						UV0.Add(FVector2D(yaxis*(index1 + 0), xaxis*(index + 0)));
						UV0.Add(FVector2D(yaxis*(index1 + 1), xaxis*(index + 0)));
						UV0.Add(FVector2D(yaxis*(index1 + 0), xaxis*(index + 1)));
						UV0.Add(FVector2D(yaxis*(index1 + 1), xaxis*(index + 1)));
					}
					index1++;
				}

				index++;
			}
		}
		branchindex_i++;
	}
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, false);

}

bool ATreeGenerate::checkleaf(int type) {
	bool leaf = true;
	int index = 0;
	while (index < MapBranchtoLeavesbyType.Num()) {
		if (type == MapBranchtoLeavesbyType[index]) {
			leaf = false;
		}
		index++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("check L %d %d"),leaf,type );
	return leaf;
}

bool ATreeGenerate::randomtime(float prob) {
	if (randomprob() < prob) {
		return true;
	}
	else return false;
}

int ATreeGenerate::indexleaf(int index) {
	int token = 0;
	int retoken = -2;
	while (token < MapBranchtoLeavesbyType.Num()) {
		if (MapBranchtoLeavesbyType[token] == index) {
			retoken = token;
		}
		token++;
	}
	return retoken;
}

void ATreeGenerate::createleaves() {
	TArray<leaf*> allLeaves;
	int branchindex_i = 0;
	while (branchindex_i<branchindex.Num()) {
		if (!checkleaf(branchindex[branchindex_i]->type)) {
			int leaftype = indexleaf(branchindex[branchindex_i]->type);
			if (leaftype != -2) {
				TArray<float> sleaf = branchindex[branchindex_i]->start;
				TArray<float> sangle;// = branchindex[branchindex_i]->angle;

				TArray<float> start_leaf = branchindex[branchindex_i]->start;
				TArray<float> stop_leaf = branchindex[branchindex_i]->stop;
				TArray<float> vector_leaf;
				vector_leaf.Add(stop_leaf[0] - start_leaf[0]);
				vector_leaf.Add(stop_leaf[1] - start_leaf[1]);
				vector_leaf.Add(stop_leaf[2] - start_leaf[2]);
				float leaf_token = vector_leaf[0] * vector_leaf[0] + vector_leaf[1] * vector_leaf[1];
				float leaf_token2;
				leaf_token = sqrt(leaf_token);
				float ang = 0;
				if (vector_leaf[0] > 0 && vector_leaf[1] > 0) {
					ang = acos(vector_leaf[0] / leaf_token);
					ang = ang * 180 / PI;
				}
				else if (vector_leaf[0] <= 0 && vector_leaf[1] > 0) {
					ang = acos(-1 * vector_leaf[0] / leaf_token);
					ang = ang * 180 / PI;
					ang = 180 - ang;
					if (vector_leaf[0] == 0) {
						ang = 90;
					}
				}
				else if (vector_leaf[0] > 0 && vector_leaf[1] <= 0) {
					ang = acos(vector_leaf[0] / leaf_token);
					ang = ang * 180 / PI;
					ang = 360 - ang;
					if (vector_leaf[1] == 0) {
						ang = 0;
					}
				}
				else if (vector_leaf[0] <= 0 && vector_leaf[1] <= 0) {
					if (vector_leaf[0] == 0 && vector_leaf[1] == 0) {
						ang = 0;
					}
					else {
						ang = acos(-1 * vector_leaf[0] / leaf_token);
						ang = ang * 180 / PI;
						ang = ang + 180;
					}
				}

				leaf_token = vector_leaf[1] * vector_leaf[1] + vector_leaf[2] * vector_leaf[2];
				leaf_token = sqrt(leaf_token);

				if (vector_leaf[2] > 0 && vector_leaf[1] > 0) {
					leaf_token2 = acos(vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
				}
				else if (vector_leaf[2] <= 0 && vector_leaf[1] > 0) {
					leaf_token2 = acos(-1 * vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
					leaf_token2 = 180 - leaf_token2;
					if (vector_leaf[2] == 0) {
						leaf_token2 = 90;
					}
				}
				else if (vector_leaf[2] > 0 && vector_leaf[1] <= 0) {
					leaf_token2 = acos(vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
					leaf_token2 = 360 - leaf_token2;
					if (vector_leaf[1] == 0) {
						leaf_token2 = 0;
					}
				}
				else if (vector_leaf[2] <= 0 && vector_leaf[1] <= 0) {
					if (vector_leaf[2] == 0 && vector_leaf[1] == 0) {
						leaf_token2 = 0;
					}
					else {
						leaf_token2 = acos(-1 * vector_leaf[2] / leaf_token);
						leaf_token2 = leaf_token2 * 180 / PI;
						leaf_token2 = leaf_token2 + 180;
					}
				}
				sangle.Add(leaf_token2);

				leaf_token = vector_leaf[0] * vector_leaf[0] + vector_leaf[2] * vector_leaf[2];
				leaf_token = sqrt(leaf_token);

				if (vector_leaf[2] > 0 && vector_leaf[0] > 0) {
					leaf_token2 = acos(vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
				}
				else if (vector_leaf[2] <= 0 && vector_leaf[0] > 0) {
					leaf_token2 = acos(-1 * vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
					leaf_token2 = 180 - leaf_token2;
					if (vector_leaf[2] == 0) {
						leaf_token2 = 90;
					}
				}
				else if (vector_leaf[2] > 0 && vector_leaf[0] <= 0) {
					leaf_token2 = acos(vector_leaf[2] / leaf_token);
					leaf_token2 = leaf_token2 * 180 / PI;
					leaf_token2 = 360 - leaf_token2;
					if (vector_leaf[0] == 0) {
						leaf_token2 = 0;
					}
				}
				else if (vector_leaf[2] <= 0 && vector_leaf[0] <= 0) {
					if (vector_leaf[2] == 0 && vector_leaf[0] == 0) {
						leaf_token2 = 0;
					}
					else {
						leaf_token2 = acos(-1 * vector_leaf[2] / leaf_token);
						leaf_token2 = leaf_token2 * 180 / PI;
						leaf_token2 = leaf_token2 + 180;
					}
				}
				sangle.Add(leaf_token2);
				//UE_LOG(LogTemp, Warning, TEXT("fuck"));
				//UE_LOG(LogTemp, Warning, TEXT("old     : %f %f %f"), branchindex[branchindex_i]->angle[0], branchindex[branchindex_i]->angle[1], branchindex[branchindex_i]->angle[2]);
				//UE_LOG(LogTemp, Warning, TEXT("vector  : %f %f %f"), vector_leaf[0], vector_leaf[1], vector_leaf[2]);
				//UE_LOG(LogTemp, Warning, TEXT("start   : %f %f %f"), start_leaf[0], start_leaf[1], start_leaf[2]);
				//UE_LOG(LogTemp, Warning, TEXT("stop    : %f %f %f"), stop_leaf[0], stop_leaf[1], stop_leaf[2]);
				//UE_LOG(LogTemp, Warning, TEXT("angle   : %f %f %f"), ang, sangle[0], sangle[1]);
				if (LeavesRotateRandom[leaftype]) {
					ang = randomprob() * 360;
				}
				else {
					ang = ang + 90 + LeavesRotate[leaftype];
				}
				sangle[0] = -sangle[0];
				sangle[1] = -sangle[1];

				float topleftx = -(LeavesWidth[leaftype] / 2);
				float toplefty = 0;
				float topleftz = LeavesHeight[leaftype];
				float toprightx = (LeavesWidth[leaftype] / 2);
				float toprighty = 0;
				float toprightz = LeavesHeight[leaftype];
				float botleftx = -(LeavesWidth[leaftype] / 2);
				float botlefty = 0;
				float botleftz = 0;
				float botrightx = (LeavesWidth[leaftype] / 2);
				float botrighty = 0;
				float botrightz = 0;
				//float ang = branchindex[branchindex_i]->data->alpha;
				float rad;
				/*
				bool angb = true;
				while (angb) {
				if (ang > 360) {
				ang = ang - 360;
				}
				else if (ang > 270 && ang <= 360) {
				ang = -1 * (ang - 180);
				}
				else if (ang > 180 && ang <= 270) {
				ang = ang - 180;
				}
				else if (ang > 90 && ang <= 180) {
				ang = -1 * (ang - 180);
				}
				else if (ang >= 0 && ang <= 90) {
				angb = false;
				}
				else if (ang < 0) {
				ang = ang + 360;
				}
				}
				*/
				rad = sqrt(topleftx*topleftx + toplefty*toplefty);
				float topleftx0 = rad*cos(ang * PI / 180);
				float toplefty0 = rad*sin(ang * PI / 180);
				float topleftz0 = topleftz;
				rad = sqrt(toprightx*toprightx + toprighty*toprighty);
				float toprightx0 = -1 * rad*cos(ang * PI / 180);
				float toprighty0 = -1 * rad*sin(ang * PI / 180);
				float toprightz0 = toprightz;
				rad = sqrt(botleftx*botleftx + botlefty*botlefty);
				float botleftx0 = rad*cos(ang * PI / 180);
				float botlefty0 = rad*sin(ang * PI / 180);
				float botleftz0 = botleftz;
				rad = sqrt(botrightx*botrightx + botrighty*botrighty);
				float botrightx0 = -1 * rad*cos(ang * PI / 180);
				float botrighty0 = -1 * rad*sin(ang * PI / 180);
				float botrightz0 = botrightz;


				rad = sqrt(topleftz0*topleftz0 + toplefty0*toplefty0);
				ang = (acos(toplefty0 / rad) * 180 / PI) + sangle[0];
				float topleftx1 = topleftx0;
				float toplefty1 = rad*cos(ang*PI / 180);
				float topleftz1 = rad*sin(ang*PI / 180);
				rad = sqrt(toprightz0*toprightz0 + toprighty0*toprighty0);
				ang = (acos(toprighty0 / rad) * 180 / PI) + sangle[0];
				float toprightx1 = toprightx0;
				float toprighty1 = rad*cos(ang*PI / 180);
				float toprightz1 = rad*sin(ang*PI / 180);
				rad = sqrt(botleftz0*botleftz0 + botlefty0*botlefty0);
				ang = (acos(botlefty0 / rad) * 180 / PI) + sangle[0];
				float botleftx1 = botleftx0;
				float botlefty1 = rad*cos(ang*PI / 180);
				float botleftz1 = rad*sin(ang*PI / 180);
				rad = sqrt(botrightz0*botrightz0 + botrighty0*botrighty0);
				ang = (acos(botrighty0 / rad) * 180 / PI) + sangle[0];
				float botrightx1 = botrightx0;
				float botrighty1 = rad*cos(ang*PI / 180);
				float botrightz1 = rad*sin(ang*PI / 180);


				float token1 = topleftz1;
				float token2 = topleftx1;
				rad = sqrt(token1*token1 + token2*token2);
				ang = (acos(sqrt(token2*token2) / rad) * 180 / PI);
				if (token2 < 0 && token1 < 0) {
					ang = 180 + ang;
				}
				else if (token2 < 0 && token1 > 0) {
					ang = 180 - ang;
				}
				else if (token2 > 0 && token1 < 0) {
					ang = -ang;
				}
				ang = ang + sangle[1];
				float topleftx2 = rad*cos(ang*PI / 180);
				float toplefty2 = toplefty1;
				float topleftz2 = rad*sin(ang*PI / 180);
				token1 = toprightz1;
				token2 = toprightx1;
				rad = sqrt(token1*token1 + token2*token2);
				ang = (acos(sqrt(token2*token2) / rad) * 180 / PI);
				if (token2 < 0 && token1 < 0) {
					ang = 180 + ang;
				}
				else if (token2 < 0 && token1 > 0) {
					ang = 180 - ang;
				}
				else if (token2 > 0 && token1 < 0) {
					ang = -ang;
				}
				ang = ang + sangle[1];
				float toprightx2 = rad*cos(ang*PI / 180);
				float toprighty2 = toprighty1;
				float toprightz2 = rad*sin(ang*PI / 180);
				token1 = botleftz1;
				token2 = botleftx1;
				rad = sqrt(token1*token1 + token2*token2);
				ang = (acos(sqrt(token2*token2) / rad) * 180 / PI);
				if (token2 < 0 && token1 < 0) {
					ang = 180 + ang;
				}
				else if (token2 < 0 && token1 > 0) {
					ang = 180 - ang;
				}
				else if (token2 > 0 && token1 < 0) {
					ang = -ang;
				}
				ang = ang + sangle[1];
				float botleftx2 = rad*cos(ang*PI / 180);
				float botlefty2 = botlefty1;
				float botleftz2 = rad*sin(ang*PI / 180);
				token1 = botrightz1;
				token2 = botrightx1;
				rad = sqrt(token1*token1 + token2*token2);
				ang = (acos(sqrt(token2*token2) / rad) * 180 / PI);
				if (token2 < 0 && token1 < 0) {
					ang = 180 + ang;
				}
				else if (token2 < 0 && token1 > 0) {
					ang = 180 - ang;
				}
				else if (token2 > 0 && token1 < 0) {
					ang = -ang;
				}
				ang = ang + sangle[1];
				float botrightx2 = rad*cos(ang*PI / 180);
				float botrighty2 = botrighty1;
				float botrightz2 = rad*sin(ang*PI / 180);
				leaf* myleaf = new leaf();
				myleaf->topleftx = topleftx2 + sleaf[0];
				myleaf->toprightx = toprightx2 + sleaf[0];
				myleaf->botleftx = botleftx2 + sleaf[0];
				myleaf->botrightx = botrightx2 + sleaf[0];
				myleaf->toplefty = toplefty2 + sleaf[1];
				myleaf->toprighty = toprighty2 + sleaf[1];
				myleaf->botlefty = botlefty2 + sleaf[1];
				myleaf->botrighty = botrighty2 + sleaf[1];
				myleaf->topleftz = topleftz2 + sleaf[2];
				myleaf->toprightz = toprightz2 + sleaf[2];
				myleaf->botleftz = botleftz2 + sleaf[2];
				myleaf->botrightz = botrightz2 + sleaf[2];
				myleaf->type = leaftype;
				allLeaves.Add(myleaf);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("index not found"));
			}
		}
		branchindex_i++;
	}
	int leavetype_index = 0;
	while (leavetype_index < LeavesMaterial.Num()) {
		int leaves_index = 0;
		TArray<FVector> vertices;
		TArray<int> Triangles;
		TArray<FVector> normals;
		TArray<FVector2D> UV0;
		TArray<FLinearColor> vertexColors;
		TArray<FProcMeshTangent> tangents;
		while (leaves_index < allLeaves.Num()) {
			if (allLeaves[leaves_index]->type == leavetype_index) {
				FVector v1 = FVector(allLeaves[leaves_index]->topleftx, allLeaves[leaves_index]->toplefty, allLeaves[leaves_index]->topleftz);
				FVector v2 = FVector(allLeaves[leaves_index]->toprightx, allLeaves[leaves_index]->toprighty, allLeaves[leaves_index]->toprightz);
				FVector v3 = FVector(allLeaves[leaves_index]->botleftx, allLeaves[leaves_index]->botlefty, allLeaves[leaves_index]->botleftz);
				FVector v4 = FVector(allLeaves[leaves_index]->botrightx, allLeaves[leaves_index]->botrighty, allLeaves[leaves_index]->botrightz);
				vertices.Add(v1);
				vertices.Add(v2);
				vertices.Add(v3);
				vertices.Add(v4);
				int tri = vertices.Num();
				Triangles.Add(tri - 4);
				Triangles.Add(tri - 2);
				Triangles.Add(tri - 1);
				Triangles.Add(tri - 4);
				Triangles.Add(tri - 1);
				Triangles.Add(tri - 3);
				Triangles.Add(tri - 3);
				Triangles.Add(tri - 1);
				Triangles.Add(tri - 2);
				Triangles.Add(tri - 3);
				Triangles.Add(tri - 2);
				Triangles.Add(tri - 4);
				normals.Add(FVector(1, 0, 0));
				normals.Add(FVector(1, 0, 0));
				normals.Add(FVector(1, 0, 0));
				normals.Add(FVector(1, 0, 0));
				vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
				vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
				vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
				vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
				tangents.Add(FProcMeshTangent(0, 1, 0));
				tangents.Add(FProcMeshTangent(0, 1, 0));
				tangents.Add(FProcMeshTangent(0, 1, 0));
				tangents.Add(FProcMeshTangent(0, 1, 0));
				UV0.Add(FVector2D(0, 0));
				UV0.Add(FVector2D(1, 0));
				UV0.Add(FVector2D(0, 1));
				UV0.Add(FVector2D(1, 1));
			}
			leaves_index++;
		}
		mesh->CreateMeshSection_LinearColor(LeavesMaterial[leavetype_index], vertices, Triangles, normals, UV0, vertexColors, tangents, false);
		leavetype_index++;
	}
}