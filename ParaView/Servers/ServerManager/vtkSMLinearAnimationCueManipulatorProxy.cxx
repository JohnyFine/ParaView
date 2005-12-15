/*=========================================================================

  Program:   ParaView
  Module:    vtkSMLinearAnimationCueManipulatorProxy.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMLinearAnimationCueManipulatorProxy.h"

#include "vtkObjectFactory.h"
#include "vtkSMAnimationCueProxy.h"
#include "vtkSMDomain.h"
#include "vtkSMProxy.h"
#include "vtkSMProperty.h"
#include "vtkClientServerID.h"

vtkCxxRevisionMacro(vtkSMLinearAnimationCueManipulatorProxy, "1.4");
vtkStandardNewMacro(vtkSMLinearAnimationCueManipulatorProxy);

//----------------------------------------------------------------------------
vtkSMLinearAnimationCueManipulatorProxy::vtkSMLinearAnimationCueManipulatorProxy()
{
  this->StartValue = 0.0;
  this->EndValue = 0.0;
}

//----------------------------------------------------------------------------
vtkSMLinearAnimationCueManipulatorProxy::~vtkSMLinearAnimationCueManipulatorProxy()
{
}

//----------------------------------------------------------------------------
void vtkSMLinearAnimationCueManipulatorProxy::UpdateValue(double currenttime,
  vtkSMAnimationCueProxy* cueproxy)
{
  double vmax = this->EndValue;
  double vmin = this->StartValue;
  double value = vmin + currenttime * (vmax - vmin);
 
  vtkSMDomain *domain = cueproxy->GetAnimatedDomain();
  vtkSMProperty *property = cueproxy->GetAnimatedProperty();
  vtkSMProxy *proxy = cueproxy->GetAnimatedProxy();

  if (domain && property)
    {
    domain->SetAnimationValue(property, cueproxy->GetAnimatedElement(),
      value);
    }
  if (proxy)
    {
    proxy->UpdateVTKObjects();
    }
  this->InvokeEvent(vtkSMAnimationCueManipulatorProxy::StateModifiedEvent);
}

//----------------------------------------------------------------------------
void vtkSMLinearAnimationCueManipulatorProxy::SaveInBatchScript(ofstream* file)
{
  this->Superclass::SaveInBatchScript(file);

  *file << "  [$pvTemp" << this->GetSelfIDAsString() 
        << " GetProperty StartValue]"
        << " SetElements1 " << this->StartValue << endl;
  *file << "  [$pvTemp" << this->GetSelfIDAsString() 
        << " GetProperty EndValue]"
        << " SetElements1 " << this->EndValue << endl;
  *file << "  $pvTemp" << this->GetSelfIDAsString() 
        << " UpdateVTKObjects" << endl;
  *file << endl; 
}

//----------------------------------------------------------------------------
void vtkSMLinearAnimationCueManipulatorProxy::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "StartValue: " << this->StartValue << endl;
  os << indent << "EndValue: " << this->EndValue << endl;
}

//----------------------------------------------------------------------------

