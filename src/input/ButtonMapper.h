/*
 *      Copyright (C) 2015 Team XBMC
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

#include "LibretroDevice.h" // for libretro_device_t

#include <string>

// TODO: Make this class generic and move XML-specific stuff to xml subfolder
class TiXmlDocument;
class TiXmlElement;

namespace ADDON { class CHelper_libXBMC_addon; }

namespace LIBRETRO
{
  class CButtonMapper
  {
  private:
    CButtonMapper(void);

  public:
    static CButtonMapper& Get(void);

    libretro_device_t GetLibretroType(const std::string& strControllerId);

    int GetLibretroIndex(const std::string& strControllerId, const std::string& strFeatureName);

    std::string GetControllerFeature(const std::string& strControllerId, const std::string& strLibretroFeature);

  private:
    bool LoadButtonMap(void);

    const TiXmlElement* GetControllerNode(const std::string& strControllerId);
    const TiXmlElement* GetFeatureNode(const std::string& strControllerId, const std::string& strFeatureName);

    bool                          m_bLoadAttempted;
    TiXmlDocument*                m_buttonMapXml;
    ADDON::CHelper_libXBMC_addon* m_addon;
  };
}
