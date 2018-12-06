// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Node.h"
#include "TreeGenerate.generated.h"

USTRUCT()
struct FBranch {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, Category = Branch)
		int From;
	UPROPERTY(EditAnywhere, Category = Branch)
		int Child;
	UPROPERTY(EditAnywhere, Category = Branch)
		int Times;
	UPROPERTY(EditAnywhere, Category = Branch)
		float Phyllotaxis;
	UPROPERTY(EditAnywhere, Category = Branch)
		float Probability;
	UPROPERTY(EditAnywhere, Category = Branch)
		float HeightMaximum;
	UPROPERTY(EditAnywhere, Category = Branch)
		float HeightMinimum;
	UPROPERTY(EditAnywhere, Category = Branch)
		float Radius;
	UPROPERTY(EditAnywhere, Category = Branch)
		float RadiusDecrease;
	UPROPERTY(EditAnywhere, Category = Branch)
		float AngleMax;
	UPROPERTY(EditAnywhere, Category = Branch)
		float AngleMin;
	FBranch()
	{
		From = -1;
		Child = 0;
		Times = 0;
		Phyllotaxis = 0;
		Probability = 1;
		HeightMaximum = 0;
		HeightMinimum = 0;
		Radius = 0;
		RadiusDecrease = 0;
		AngleMax = 0;
		AngleMin = 0;
	}
};

USTRUCT()
struct FLeaves1 {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = Leaf)
		int From;
	UPROPERTY(EditAnywhere, Category = Leaf)
		int LeafMaterial;
	UPROPERTY(EditAnywhere, Category = Leaf)
		float Height;
	UPROPERTY(EditAnywhere, Category = Leaf)
		float Width;
	UPROPERTY(EditAnywhere, Category = Leaf)
		float Rotate;
	UPROPERTY(EditAnywhere, Category = Leaf)
		bool RotateRandom;
	FLeaves1()
	{
		From = -2;
		LeafMaterial = 1;
		Height = 0;
		Width = 0;
		Rotate = 0;
		RotateRandom = true;
	}
};

UCLASS()
class MAINCHARACTERFPS2_API ATreeGenerate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreeGenerate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void CreateTree();
	virtual void CreateTreewithjoint();
	virtual bool randomtime(float prob);
	virtual float randomprob();
	virtual void branchplot();
	virtual bool checkleaf(int type);
	virtual void createleaves();
	virtual int indexleaf(int index);
	virtual void inputconverter();

	Node* root;

	struct leaf {
		float topleftx;
		float toprightx;
		float botleftx;
		float botrightx;
		float toplefty;
		float toprighty;
		float botlefty;
		float botrighty;
		float topleftz;
		float toprightz;
		float botleftz;
		float botrightz;
		int type;
	};

	//TArray<leaf*> allLeaves1;
	TArray<Node::TreeNode*> branchindex;

	//-1 root
	TArray<int> BranchFrom;
	TArray<int> BranchPerLoop;
	TArray<int> BranchTime;
	TArray<float> Phyllotaxis;
	TArray<float> BranchProb;
	TArray<float> BranchHeightMax;
	TArray<float> BranchHeightMin;
	TArray<float> BranchRadiusStart;
	TArray<float> BranchRadiusdecrease;
	TArray<int> BranchAngleMax;
	TArray<int> BranchAngleMin;
	TArray<int> MapBranchtoLeavesbyType;
	TArray<int> LeavesMaterial;
	TArray<float> LeavesHeight;
	TArray<float> LeavesWidth;
	TArray<float> LeavesRotate;
	TArray<bool> LeavesRotateRandom;

	UPROPERTY(EditAnywhere, Category = TreeGenerate)
		TArray<FBranch> branch;
	UPROPERTY(EditAnywhere, Category = TreeGenerate)
		TArray<FLeaves1> leaves;

	UPROPERTY(EditAnywhere, Category = Materials)
		UProceduralMeshComponent * mesh;
};
