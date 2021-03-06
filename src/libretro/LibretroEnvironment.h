/*
 *      Copyright (C) 2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "audio/AudioStream.h"
#include "video/VideoStream.h"

#include "kodi/kodi_game_types.h"
#include "p8-platform/threads/mutex.h"

#include <map>
#include <string>
#include <vector>

namespace ADDON { class CHelper_libXBMC_addon; }
class CHelper_libKODI_game;

namespace LIBRETRO
{
  class CClientBridge;
  class CLibretroDLL;

  class CLibretroEnvironment
  {
  public:
    static CLibretroEnvironment& Get(void);

    void Initialize(ADDON::CHelper_libXBMC_addon* xbmc, CHelper_libKODI_game* frontend, CLibretroDLL* client, CClientBridge* clientBridge);
    void Deinitialize(void);

    ADDON::CHelper_libXBMC_addon* GetXBMC(void)         { return m_xbmc; }
    CHelper_libKODI_game*         GetFrontend(void)     { return m_frontend; }
    CLibretroDLL*                 GetClient(void)       { return m_client; }
    CClientBridge*                GetClientBridge(void) { return m_clientBridge; }

    CVideoStream& Video(void) { return m_videoStream; }
    CAudioStream& Audio(void) { return m_audioStream; }

    game_system_av_info GetSystemInfo(void) const { return m_systemInfo; }
    void UpdateSystemInfo(game_system_av_info info) { m_systemInfo = info; }

    /*!
     * Returns the pixel format set by the libretro core. Instead of forwarding
     * this to the frontend, we store the value and report it on calls to
     * VideoRefresh().
     */
    GAME_PIXEL_FORMAT GetVideoFormat(void) const { return m_videoFormat; }

    /*!
     * Invoked when XBMC transfers a setting to the add-on.
     */
    void SetSetting(const char* name, const char* value);

    bool EnvironmentCallback(unsigned cmd, void* data);

  private:
    CLibretroEnvironment(void);

    ADDON::CHelper_libXBMC_addon* m_xbmc;
    CHelper_libKODI_game*         m_frontend;
    CLibretroDLL*                 m_client;
    CClientBridge*                m_clientBridge;
    CVideoStream                  m_videoStream;
    CAudioStream                  m_audioStream;

    game_system_av_info m_systemInfo;
    GAME_PIXEL_FORMAT   m_videoFormat;

    std::map<std::string, std::vector<std::string> > m_variables; // Record the variables reported by libretro core (key -> values)
    std::map<std::string, std::string>               m_settings;  // Record the settings reported by XBMC (key -> current value)
    volatile bool                                    m_bSettingsChanged;
    P8PLATFORM::CMutex                               m_settingsMutex;
  };
} // namespace LIBRETRO
