/*=============================================================================
XMOTO

This file is part of XMOTO.

XMOTO is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

XMOTO is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XMOTO; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=============================================================================*/

#include "Game.h"
#include "StateDownloadGhost.h"
#include "StateReplaying.h"
#include "thread/DownloadGhostThread.h"
#include "helpers/Log.h"

StateDownloadGhost::StateDownloadGhost(GameApp* pGame,
				       std::string levelId,
				       bool launchReplaying,
				       bool drawStateBehind,
				       bool updateStatesBehind)
  : StateUpdate(pGame, drawStateBehind, updateStatesBehind)
{
  m_pThread         = new DownloadGhostThread(this, levelId);
  m_name            = "StateDownloadGhost";
  m_replayName      = "";
  m_launchReplaying = launchReplaying;
  // we don't want a message box on failure.
  m_messageOnFailure = launchReplaying;
}

StateDownloadGhost::~StateDownloadGhost()
{
}

void StateDownloadGhost::setReplay(std::string replayName)
{
  m_replayName = replayName;
}

void StateDownloadGhost::callAfterThreadFinished(int threadResult)
{
  m_msg = ((DownloadGhostThread*)m_pThread)->getMsg();

  if(threadResult == 0 && m_launchReplaying == true){
    std::string msg = "Replay to play: " + m_replayName;
    Logger::Log(msg.c_str());
    m_pGame->getStateManager()->replaceState(new StateReplaying(m_pGame, m_replayName));
  }
  else{
    m_pGame->getStateManager()->sendAsynchronousMessage("GHOST_DOWNLOADED");
  }
}