// Copyright Eveline Gomes.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
   /** 
   * Use NetUpdateFrequency to set how often the server tries to update clients. That's useful to sync up clients with the server version 
   *  as changes occur on ther server for the player state. So any variables that should be replicated, they'll update and the server will
   *  try to meet this net update frequency if it can.
   * Normally this net update frequency is low, but as we need to have our ability system component and atribute set in the player state,
   *  we should make this update faster, set it to a higher value! That's why we're using 100 here.
   */
   NetUpdateFrequency = 100.f;
}
