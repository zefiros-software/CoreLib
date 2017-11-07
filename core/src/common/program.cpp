/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Zefiros Software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */

#include "manager/applicationManager.h"
#include "manager/systemManager.h"
#include "manager/logManager.h"

#include "common/program.h"
#include "common/format.h"
#include "config.h"


Program::Program( S32 argc, const char **argv ) noexcept
    : mDesc({{"", String::Place("{} - {}", std::string(PROGRAM_NAME), std::string(PROGRAM_COPYRIGHT))}}),
      mIsHelpCommand(false),
      mCLIParsed(false),
      mIsInitialised(false),
      mArgc(argc),
      mArgv(argv)
{
    // Create a system manager and provide it for global access
    // using the service locater pattern.
    SystemManager* systemManager = new SystemManager(mArgc, mArgv);

    // Provide our service locator
    SystemManager::Get(systemManager);

    systemManager->RegisterManagers();
}

Program::~Program()
{
    Shutdown();
}

void Program::Update()
{
    if ( !mIsHelpCommand )
    {
        if ( !mIsInitialised )
        {
            Init();
        }

        SystemManager::Get()->GetManagers()->system->Update();
    }
}

void Program::Init( Console::LogMode logMode )
{
    mCLIParsed = ParseCLI();
    if (mCLIParsed)
    {
        SystemManager::Get()->GetManagers()->log->SetMode(logMode);
        SystemManager::Get()->GetManagers()->system->Initialise();

        mIsInitialised = true;
    }
    else
    {
        SystemManager::Get()->GetManagers()->log->SetMode(Console::LogMode::Disabled);
    }
}

bool Program::IsRunning() const noexcept
{
    if ( mIsHelpCommand || !mCLIParsed )
    {
        return false;
    }

    if ( !mIsInitialised )
    {
        return true;
    }

    return SystemManager::Get()->GetManagers()->application->IsRunning();
}

void Program::AddOption(const std::string &val, const std::string &description, const std::string &group /*= "" */)
{
    mDesc[group].add_options()(val.c_str(), description.c_str());
}

bool Program::HasOption(const std::string &opt) const
{
    return mVM.count(opt) != 0;
}

void Program::Shutdown()
{
    SystemManager::Get()->Release();
}

bool Program::ParseCLI()
{
    try
    {
        mDesc[""].add_options()("help,h", "Shows this help screen");

        boost::program_options::options_description tot;

        for (auto &desc : mDesc)
        {
            tot.add(desc.second);
        }

        boost::program_options::store(parse_command_line(mArgc, mArgv, tot), mVM);
        boost::program_options::notify(mVM);

        if (mVM.count("help"))
        {
            std::cout << tot;
            mIsHelpCommand = true;
            return false;
        }
    }
    catch (const boost::program_options::error &ex)
    {
        std::cerr << ex.what() << '\n';
        return false;
    }

    return true;
}
