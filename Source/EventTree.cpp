//
//  EventTree.cpp
//  PowerPlant
//
//  Created by Maxwell Maurice on 4/21/15.
//  Copyright (c) 2015 Maxwell Maurice. All rights reserved.
//

#include "EventTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>


//Event tree that stores all the possible events and damages that can happen to the plant.
EventTree::EventTree(){
    root = NULL;
}
EventTree::~EventTree(){}
//Sets up the tree and reads in a special .txt file specific to the plant. The tree is filled in a semi tree balanced order below based off IDArray.
void EventTree::TreeSetup(std::string PlantTyp){
    int index =0;
    int IDArray[25] = {0,15,21,7,4,9,23,20,5,8,10,6,22,16,24,3,11,2,12,13,1,14,19,17,18};
    std::ifstream in_stream;
    in_stream.open(PlantTyp+"Events.txt");
    std::string IC,PRC,PPC,BC,T;
    while (!in_stream.eof()){
        getline(in_stream, IC, ',');
        getline(in_stream, PRC, ',');
        getline(in_stream, PPC, ',');
        getline(in_stream, BC, ',');
        getline(in_stream, T);
        addEventNode(atoi(IC.c_str()),atoi(PRC.c_str()),atoi(PPC.c_str()),atoi(BC.c_str()),T,IDArray[index]);
        index++;
    }
}

//Adds event node to tree
void EventTree::addEventNode(int IC,int PRC,int PPC,int BC,std::string T,int ID){
    if (ID == 0) {
        root = new EventNode(IC, PRC, PPC, BC,T,ID);//gives the root memory
        root->leftChild = NULL;
        root->rightChild = NULL;
    }else{
        EventNode* A = new EventNode(IC, PRC, PPC, BC,T,ID);
        EventNode* B = root;
        A->leftChild = NULL;
        A->rightChild = NULL;
        while (true) {
            if (B->leftChild == NULL) {
                if (ID<B->TreeID) {
                    B->leftChild = A;
                    break;
                }
            }
            if (B->rightChild == NULL) {
                if (ID>=B->TreeID) {
                    B->rightChild = A;
                    break;
                }
            }
            
            if (ID<B->TreeID) {
                B = B->leftChild;
            }else{
                B = B->rightChild;
            }
        }
    }
}
//serches tree based off of integer ID
EventNode* EventTree::searchEventTree(EventNode* node, int ID){
    if (node == NULL)
        return NULL;
    else if (node->TreeID == ID)
        return node;
    else{
        if(ID<node->TreeID)
            return searchEventTree(node->leftChild,ID);
        else
            return searchEventTree(node->rightChild,ID);
    }
}
//Returns a event at random
EventNode* EventTree::getRandomEvent(){
    int RandomEvent = rand() % 25;
    srand (static_cast<unsigned int>(time(NULL)));
    
    EventNode* CurrentEvent = searchEventTree(root, RandomEvent);
    while (CurrentEvent->Marked == true) {
        int RandomEvent = rand() % 25;
        srand (static_cast<unsigned int>(time(NULL)));
        CurrentEvent = searchEventTree(root, RandomEvent);
    }
    return CurrentEvent;
}

//Executes event and determains if your management skills are high enough to solve the sistuation
PowerPlant EventTree::EventHandler(PowerPlant UserPlant, EventNode* CurrentEvent, AssistantManager AM){
    std::cout<<"\n"<<CurrentEvent->Text<<std::endl;
    if (CurrentEvent->InvestmentChange != 0) {
        CurrentEvent->Marked=true;
        if ((CurrentEvent->InvestmentChange)/10 >= (UserPlant.InvestmentBudget)/(10-AM.InvestmentAbility)) {
            std::cout<<"Unfortuantly Your budget for Investment was not high enough"<<"\n"<<"and your Assistaint manager could not resolved the situation"<<std::endl;
            UserPlant.InvestmentScore -=CurrentEvent->InvestmentChange;
        }else{
            std::cout<<"Takefully Your budget for Investment was high enough"<<"\n"<<"and your Assistaint manager resolved the situation"<<std::endl;
            UserPlant.InvestmentScore +=CurrentEvent->InvestmentChange;
        }
    }
    if (CurrentEvent->PublicRelationsChange != 0) {
        CurrentEvent->Marked=true;
        if ((CurrentEvent->PublicRelationsChange)/10 >= (UserPlant.PublicRelationsScore)/(10-AM.PublicRelationsAbility)) {
            std::cout<<"Unfortuantly Your budget for Public Relations was not high enough"<<"\n"<<"and your Assistaint manager could not resolved the situation"<<std::endl;
            UserPlant.PublicRelationsScore -=CurrentEvent->PublicRelationsChange;
        }else{
            std::cout<<"Takefully Your budget for Public Relations was high enough"<<"\n"<<"and your Assistaint manager resolved the situation"<<std::endl;
            UserPlant.PublicRelationsScore +=CurrentEvent->PublicRelationsChange;
        }
    }
    if (CurrentEvent->PowerProductionChange != 0) {
        CurrentEvent->Marked=true;
        if ((CurrentEvent->PowerProductionChange)/10 >= (UserPlant.MatinanceBudget)/(10-AM.MaintenanceAbility)) {
            std::cout<<"Unfortuantly Your budget for Matinance was not high enough"<<"\n"<<"and your Assistaint manager could not resolved the situation"<<std::endl;
            UserPlant.PowerProductionScore -=CurrentEvent->PowerProductionChange;
        }else{
            std::cout<<"Takefully Your budget for Matinance was high enough"<<"\n"<<"and your Assistaint manager resolved the situation"<<std::endl;
            UserPlant.PowerProductionScore +=CurrentEvent->PowerProductionChange;
        }
    }
    if (CurrentEvent->BoardChange != 0) {
        CurrentEvent->Marked=true;
        if ((CurrentEvent->BoardChange)/10 >= (UserPlant.StaffBudget)/(10-AM.StaffManAbility)) {
            std::cout<<"Unfortuantly Your budget for your Staff was not high enough"<<"\n"<<"and your Assistaint manager could not resolved the situation"<<std::endl;
            UserPlant.BoardScore -=CurrentEvent->BoardChange;
        }else{
            std::cout<<"Takefully Your budget for your Staff was high enough"<<"\n"<<"and your Assistaint manager resolved the situation"<<std::endl;
            UserPlant.BoardScore +=CurrentEvent->BoardChange;
        }
    }
    return UserPlant;
}
