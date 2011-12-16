/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2. 
   
   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#ifndef __pqMultiViewWidget_h 
#define __pqMultiViewWidget_h

#include <QWidget>
#include "pqComponentsExport.h"

class pqMultiViewFrame;
class pqView;
class vtkSMProxy;
class vtkSMViewLayoutProxy;

/// pqMultiViewWidget is a widget that manages layout of multiple views. It
/// works together with a vtkSMViewLayoutProxy instance to keep track of the layout
/// for the views. It's acceptable to create multiple instances of
/// pqMultiViewWidget in the same application.
class PQCOMPONENTS_EXPORT pqMultiViewWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;
public:
  pqMultiViewWidget(QWidget * parent=0, Qt::WindowFlags f=0);
  virtual ~pqMultiViewWidget();

  /// Get/Set the vtkSMViewLayoutProxy instance this widget is using as the layout
  /// manager.
  void setLayoutManager(vtkSMViewLayoutProxy*);
  vtkSMViewLayoutProxy* layoutManager() const;

public slots:
  /// this forces the pqMultiViewWidget to reload its layout from the
  /// vtkSMViewLayoutProxy instance. One does not need to call this method
  /// explicitly, it is called automatically when the layoutManager is modified.
  void reload();

  /// assigns a frame to the view. This assumes that the view not already been
  /// placed in a frame. This will try to locate an empty frame, if possible. If
  /// no empty frames are available, it will split the active frame along its
  /// longest dimension and place the view in the newly created child-frame.
  void assignToFrame(pqView*);

  /// In a tabbed setup, when pqMultiViewWidget becomes active, this method
  /// should be called to ensure that the first view/frame in this widget is
  /// indeed made active, as the user would expect.
  void makeFrameActive();

protected slots:
  /// slots called on different signals fired by the nested frames or splitters.
  /// Note that these slots use this->sender(), hence these should not be called
  /// directly. These result in updating the layoutManager.
  void splitVertical();
  void splitHorizontal();
  void close();
  void splitterMoved();

  /// makes a frame active. This also call pqActiveObjects::setActiveView() to
  /// make the corresponding view active.
  void makeActive(pqMultiViewFrame* frame);

  /// marks the frame corresponding to the view, if present in the widget, as
  /// active. Note that this method does not fire the activeChanged() signal.
  void markActive(pqView* view);
  void markActive(pqMultiViewFrame* frame);

protected:
  /// called whenever a new frame needs to be created for a view. Note that view
  /// may be null, in which case a place-holder frame is expected. The caller
  /// takes over the ownership of the created frame and will delete/re-parent it
  /// as and when appropriate.
  virtual pqMultiViewFrame* newFrame(vtkSMProxy* view);

  /// event filter callback to detect when a sub-frame becomes active, so that
  /// we can mark it as such.
  virtual bool eventFilter(QObject* caller, QEvent* evt);

private:
  QWidget* createWidget(int, vtkSMViewLayoutProxy* layout, QWidget* parentWdg);

private:
  Q_DISABLE_COPY(pqMultiViewWidget);

  class pqInternals;
  pqInternals* Internals;
};

#endif
