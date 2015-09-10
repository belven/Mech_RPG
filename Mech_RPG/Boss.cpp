#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

bool ABoss::CanAttack(){
	return canAttack;
}

bool ABoss::CanMove(){
	return canMove;
}

void ABoss::SetCanAttack(bool newVal){
	canAttack = newVal;
}

void ABoss::SetCanMove(bool newVal){
	canMove = newVal;
}