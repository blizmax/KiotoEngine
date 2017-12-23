//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include <string>
#include <map>
#include <vector>

#include "Asset.h"

namespace Kioto::AssetsSystem // [a_vorontsov] Maybe to class and use with service locator.
{
void Init();
void Shutdown();
std::wstring GetAssetFullPath(const std::wstring& assetName);
bool CheckIfFileExist(const std::wstring& path);
bool CheckIfFileExist(const std::string& path);

void UnloadAsset(std::string assetPath);
void CleanAssets();
void RegisterAsset(const Asset* asset);

template <typename T>
T* LoadAsset(std::string assetPath);
template <typename T>
void UnloadAsset(T* asset);
template <typename T>
T* CreateUniqueCopy(const T* source);

static std::map<std::string, Asset*> m_assets;
static std::vector<Asset*> m_dynamicAssets;

template <typename T>
T* Kioto::AssetsSystem::LoadAsset(std::string assetPath)
{
    auto it = m_assets.find(assetPath);
    if (it != m_assets.end())
        return static_cast<T*>(it->second);

    if (!CheckIfFileExist(assetPath))
        return nullptr;

    Asset* newAsset = new T(assetPath);
    m_assets[assetPath] = newAsset;
    return static_cast<T*>(newAsset);
}

template <typename T>
void UnloadAsset(T* asset)
{
    auto it = std::find(m_dynamicAssets.begin(), m_dynamicAssets.end(), asset);
    if (it != m_dynamicAssets.end())
    {
        delete &(*it);
        m_dynamicAssets.erase(it);
        return;
    }
    auto mapIt = std::find_if(m_assets.begin(), m_assets.end(), [&asset](const auto& pair) { return pair.second() == asset; });
    if (mapIt != m_assets.end())
    {
        SafeDelete(mapIt->second);
        m_assets.erase(mapIt);
    }
}

template <typename T>
void CreateUniqueCopy(const T* source)
{
    Asset* res = new T(*source);
    m_dynamicAssets.push_back(res);
    return static_cast<T>(res);
}
}