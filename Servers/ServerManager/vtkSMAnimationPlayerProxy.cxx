/*=========================================================================

  Program:   ParaView
  Module:    vtkSMAnimationPlayerProxy.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMAnimationPlayerProxy.h"

#include "vtkAnimationPlayer.h"
#include "vtkClientServerStream.h"
#include "vtkCommand.h"
#include "vtkObjectFactory.h"
#include "vtkProcessModule.h"

#include <vtkstd/vector>
//----------------------------------------------------------------------------
class vtkSMAnimationPlayerProxy::vtkObserver : public vtkCommand
{
public:
  static vtkObserver* New()
    {
    return new vtkObserver();
    }
  virtual void Execute(vtkObject*, unsigned long event, void* data)
    {
    if (this->Target)
      {
      this->Target->InvokeEvent(event, data);
      }
    }
  void SetTarget(vtkSMAnimationPlayerProxy* t)
    {
    this->Target = t;
    }
private:
  vtkObserver()
    {
    this->Target = 0;
    }
  vtkSMAnimationPlayerProxy* Target;
};
//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSMAnimationPlayerProxy);
//----------------------------------------------------------------------------
vtkSMAnimationPlayerProxy::vtkSMAnimationPlayerProxy()
{
  this->Observer = vtkObserver::New();
  this->Observer->SetTarget(this);
  this->SetLocation(vtkProcessModule::CLIENT);
}

//----------------------------------------------------------------------------
vtkSMAnimationPlayerProxy::~vtkSMAnimationPlayerProxy()
{
  this->Observer->SetTarget(0);
  this->Observer->Delete();
}

//----------------------------------------------------------------------------
void vtkSMAnimationPlayerProxy::CreateVTKObjects()
{
  if (this->ObjectsCreated)
    {
    return;
    }

  this->SetLocation(vtkProcessModule::CLIENT);
  this->Superclass::CreateVTKObjects();

  vtkObject * obj = vtkObject::SafeDownCast(this->GetClientSideObject());
  obj->AddObserver(vtkCommand::StartEvent, this->Observer);
  obj->AddObserver(vtkCommand::EndEvent, this->Observer);
  obj->AddObserver(vtkCommand::ProgressEvent, this->Observer);

  if (obj->IsA("vtkCompositeAnimationPlayer"))
    {
    vtkstd::vector<vtkSMProxy*> subproxies;

    vtkSMProxy* proxy = this->GetSubProxy("SequenceAnimationPlayer");
    if (proxy)
      {
      subproxies.push_back(proxy);
      }

    proxy = this->GetSubProxy("RealtimeAnimationPlayer");
    if (proxy)
      {
      subproxies.push_back(proxy);
      }

    proxy = this->GetSubProxy("TimestepsAnimationPlayer");
    if (proxy)
      {
      subproxies.push_back(proxy);
      }



    vtkClientServerStream stream;
    for (unsigned int cc=0; cc < subproxies.size(); ++cc)
      {
      stream << vtkClientServerStream::Invoke
        << this->GetClientSideObject()
        << "AddPlayer"
        << subproxies[cc]->GetClientSideObject()
        << vtkClientServerStream::End;
      }
    // TODO execute stream with local interpretor
    // FIXME ++++++++++
    // FIXME ++++++++++
    // FIXME ++++++++++
    // FIXME ++++++++++


    }
}

//----------------------------------------------------------------------------
int vtkSMAnimationPlayerProxy::IsInPlay()
{
  if (this->ObjectsCreated && this->GetClientSideObject())
    {
    vtkAnimationPlayer* obj = vtkAnimationPlayer::SafeDownCast(
        this->GetClientSideObject());
    return obj->IsInPlay();
    }
  return 0;
}

//----------------------------------------------------------------------------
void vtkSMAnimationPlayerProxy::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

