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

#include "InputManager.h"
#include "libretro.h"
#include "LibretroEnvironment.h"

#include "kodi/libKODI_game.h"

using namespace LIBRETRO;

CInputManager& CInputManager::Get(void)
{
  static CInputManager _instance;
  return _instance;
}

libretro_device_caps_t CInputManager::GetDeviceCaps(void) const
{
  return 1 << RETRO_DEVICE_JOYPAD   |
         1 << RETRO_DEVICE_MOUSE    |
         1 << RETRO_DEVICE_KEYBOARD |
         1 << RETRO_DEVICE_LIGHTGUN |
         1 << RETRO_DEVICE_ANALOG   |
         1 << RETRO_DEVICE_POINTER;
}

void CInputManager::DeviceConnected(unsigned int port, bool bConnected, const game_input_device* connectedDevice)
{
  if (port < m_ports.size())
    m_ports[port] = CLibretroDevice(bConnected ? connectedDevice : NULL);
}

libretro_device_t CInputManager::GetDevice(unsigned int port) const
{
  libretro_device_t deviceType = 0;

  if (port < m_ports.size())
    deviceType = m_ports[port].Type();

  return deviceType;
}

bool CInputManager::OpenPort(unsigned int port)
{
  if (!CLibretroEnvironment::Get().GetFrontend())
    return false;

  if (port >= m_ports.size())
    m_ports.resize(port + 1);

  CLibretroEnvironment::Get().GetFrontend()->OpenPort(port);

  return true;
}

void CInputManager::ClosePort(unsigned int port)
{
  if (!CLibretroEnvironment::Get().GetFrontend())
    return;

  CLibretroEnvironment::Get().GetFrontend()->ClosePort(port);

  if (port < m_ports.size())
    m_ports[port].Clear();
}

void CInputManager::ClosePorts(void)
{
  for (unsigned int i = 0; i < m_ports.size(); i++)
    ClosePort(i);
}

void CInputManager::EnableAnalogSensors(unsigned int port, bool bEnabled)
{
  // TODO
}

bool CInputManager::InputEvent(unsigned int port, const game_input_event& event)
{
  bool bHandled = false;

  if (port < m_ports.size())
    bHandled = m_ports[port].InputEvent(event);

  return bHandled;
}

void CInputManager::LogInputDescriptors(const retro_input_descriptor* descriptors)
{
  /* TODO
  for (const retro_input_descriptor* descriptor = descriptors; descriptor->description != NULL; descriptor++)
  {
    switch (descriptor->device)
    {
    case RETRO_DEVICE_JOYPAD:
    case RETRO_DEVICE_MOUSE:
    case RETRO_DEVICE_KEYBOARD:
    case RETRO_DEVICE_LIGHTGUN:
    case RETRO_DEVICE_ANALOG:
    case RETRO_DEVICE_POINTER:
    case RETRO_DEVICE_JOYPAD_MULTITAP:
    case RETRO_DEVICE_LIGHTGUN_SUPER_SCOPE:
    case RETRO_DEVICE_LIGHTGUN_JUSTIFIER:
    case RETRO_DEVICE_LIGHTGUN_JUSTIFIERS:
    default:
      break;
    }
  }
  */
}

bool CInputManager::ButtonState(libretro_device_t device, unsigned int port, unsigned int buttonIndex)
{
  bool bState = false;

  if (port < m_ports.size() || OpenPort(port))
  {
    bState = m_ports[port].ButtonState(buttonIndex);
  }

  return bState;
}

int CInputManager::DeltaX(libretro_device_t device, unsigned int port)
{
  int deltaX = 0;

  if (port < m_ports.size() || OpenPort(port))
  {
    deltaX = m_ports[port].RelativePointerDeltaX();
  }

  return deltaX;
}

int CInputManager::DeltaY(libretro_device_t device, unsigned int port)
{
  int deltaY = 0;

  if (port < m_ports.size() || OpenPort(port))
  {
    deltaY = m_ports[port].RelativePointerDeltaY();
  }

  return deltaY;
}

bool CInputManager::AnalogStickState(unsigned int port, unsigned int analogStickIndex, float& x, float& y)
{
  bool bSuccess = false;

  if (port < m_ports.size() || OpenPort(port))
  {
    bSuccess = m_ports[port].AnalogStickState(analogStickIndex, x, y);
  }

  return bSuccess;
}

bool CInputManager::AbsolutePointerState(unsigned int port, unsigned int pointerIndex, float& x, float& y)
{
  bool bSuccess = false;

  if (port < m_ports.size() || OpenPort(port))
  {
    bSuccess = m_ports[port].AbsolutePointerState(pointerIndex, x, y);
  }

  return bSuccess;
}

bool CInputManager::AccelerometerState(unsigned int port, float& x, float& y, float& z)
{
  bool bSuccess = false;

  if (port < m_ports.size() || OpenPort(port))
  {
    bSuccess = m_ports[port].AccelerometerState(x, y, z);
  }

  return bSuccess;
}
