/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/



#include "AssetCache/Test/AssetCacheTest.h"
#include "AssetCache/AssetCache.h"

#include "FileSystem/DynamicMemoryFile.h"
#include "FileSystem/FileList.h"
#include "Platform/SystemTimer.h"
#include "Platform/Thread.h"


#include "Network/Base/Endpoint.h"


//
//namespace DAVA
//{
//    
//namespace AssetCache
//{
// 
//struct TestEntryDescriptor
//{
//    FilePath project;
//    FilePath outFolder;
//    ClientCacheEntry entry;
//};
// 
//    
//class ServerTest: public DAVA::AssetCache::ServerDelegate
//{
//public:
//    
//    DAVA::AssetCache::Server *server = nullptr;
//    DAVA::AssetCache::CacheDB dataBase;
//    
//public:
//    
//    ServerTest(DAVA::AssetCache::Server *_server)
//    :   server(_server)
//    ,   dataBase("/Users/victorkleschenko/Downloads/__AssetCacheTest/CacheFolder", 10 * 1024 * 1024, 1)
//    {
//        server->SetDelegate(this);
//    }
//    
//    void OnAddedToCache(const DAVA::AssetCache::CacheItemKey &key, const DAVA::AssetCache::CachedFiles &files) override
//    {
//        if(server)
//        {
//            dataBase.Insert(key, files);
//            server->FilesAddedToCache(key, true);
//        }
//    }
//    
//    void OnIsInCache(const DAVA::AssetCache::CacheItemKey &key) override
//    {
//        if(server)
//        {
//            auto entry = dataBase.Get(key);
//            bool inCache = (nullptr != entry);
//            
//            server->FilesInCache(key, inCache);
//        }
//    }
//    
//    void OnRequestedFromCache(const DAVA::AssetCache::CacheItemKey &key) override
//    {
//        if(server)
//        {
//            auto entry = dataBase.Get(key);
//            if(entry)
//            {
//                server->SendFiles(key, entry->GetFiles());
//            }
//            else
//            {
//                server->SendFiles(key, DAVA::AssetCache::CachedFiles());
//            }
//        }
//    }
//};
//
//    
//class ClientTest: public DAVA::AssetCache::ClientDelegate
//{
//    bool testIsRunning = false;
//    
//    DAVA::AssetCache::Client *client = nullptr;
//    
//    TestEntryDescriptor entry;
//    
//public:
//    
//    ClientTest(DAVA::AssetCache::Client * _client)
//        :   client(_client)
//    {
//        client->SetDelegate(this);
//    }
//    
//    void OnAddedToCache(const DAVA::AssetCache::CacheItemKey &key, bool added) override
//    {
//        testIsRunning = false;
//    }
//    
//    void OnIsInCache(const DAVA::AssetCache::CacheItemKey &key, bool isInCache) override
//    {
//    }
//    
//    void OnReceivedFromCache(const DAVA::AssetCache::CacheItemKey &key, const DAVA::AssetCache::CachedFiles &files) override
//    {
//        if(files.IsEmtpy())
//        {
//            client->AddToCache(entry.entry.GetKey(), entry.entry.GetFiles());
//        }
//        else
//        {
//            files.Save(entry.outFolder);
//            testIsRunning = false;
//        }
//    }
//    
//    
//    void Run(const TestEntryDescriptor & _entry)
//    {
//        testIsRunning = true;
//        entry = _entry;
//        
//        client->GetFromCache(entry.entry.GetKey());
//        
//        while (testIsRunning)
//        {
//            //wait;
//        }
//    }
//};
//
//
//void RunPackerTest()
//{
//// --- INITIALIZATION OF NETWORK ---
//    static const DAVA::uint16 port = 5566;
//    
//    auto server = new DAVA::AssetCache::Server();
//    server->Listen(port);
//    
//    auto client = new DAVA::AssetCache::Client();
//    client->Connect("127.0.0.1", port);
//    
//    Net::NetCore::Instance()->RestartAllControllers();
//    while(client->IsConnected() == false || server->IsConnected() == false)
//    {
//        sleep(10);
//    }
//// --- INITIALIZATION OF NETWORK ---
//    
//    
//// --- INITIALIZATION OF TEST DATA ---
//    
//    auto CreateEntryForProject = [] (const String &project, const String &outFolder)
//    {
//        TestEntryDescriptor entryDescriptor;
//        entryDescriptor.project = String("/Users/victorkleschenko/Downloads/__AssetCacheTest/") + project;
//        entryDescriptor.project.MakeDirectoryPathname();
//        
//        entryDescriptor.outFolder = entryDescriptor.project + outFolder;
//        entryDescriptor.outFolder.MakeDirectoryPathname();
//        
//        const FilePath inputFolder = entryDescriptor.project + "InFolder/";
//        const FilePath processFolder = entryDescriptor.project + "$process/";
//
//        entryDescriptor.entry.AddParam("Resource Packer 1.0");
//        entryDescriptor.entry.AddParam("RGBA8888");
//        entryDescriptor.entry.AddParam("split");
//        
//        ScopedPtr<FileList> flist(new FileList(processFolder));
//        auto count = flist->GetCount();
//        for(auto i = 0; i < count; ++i)
//        {
//            if(!flist->IsDirectory(i))
//            {
//                entryDescriptor.entry.AddFile(flist->GetPathname(i));
//            }
//        }
//        
//        uint8 digest[MD5::DIGEST_SIZE];
//        MD5::ForDirectory(inputFolder, digest, false, false);
//        entryDescriptor.entry.InvalidatePrimaryKey(digest);
//        entryDescriptor.entry.InvalidateSecondaryKey();
//        
//        return entryDescriptor;
//    };
//
//    
//    TestEntryDescriptor cacheEntry1 = CreateEntryForProject("TestProject1", "OutFolder_0");
//    TestEntryDescriptor cacheEntry2 = CreateEntryForProject("TestProject2", "OutFolder_0");
//    
//// --- INITIALIZATION OF TEST DATA ---
//    
//// --- RUNNING TEST ---
//    ServerTest serverTest(server);
//    serverTest.dataBase.Dump();
//    
//    
//    ClientTest clientTest(client);
//
//    auto TestFunc = [] (ClientTest & test, TestEntryDescriptor & entry)
//    {
//        entry.entry.files.LoadFiles();
//        test.Run(entry);
//        
//        entry.entry.files.UnloadFiles();
//        test.Run(entry);
//    };
//    
//    
//
//    TestFunc(clientTest, cacheEntry1);
//    serverTest.dataBase.Dump();
//    
//    TestFunc(clientTest, cacheEntry2);
//    serverTest.dataBase.Dump();
//    
//    // --- RUNNING TEST ---
//    
//    Net::NetCore::Instance()->DestroyAllControllersBlocked();
//    Net::NetCore::Instance()->UnregisterAllServices();
//    
//    SafeDelete(client);
//    SafeDelete(server);
//}
//    
//}; // end of namespace AssetCache
//}; // end of namespace DAVA
//
