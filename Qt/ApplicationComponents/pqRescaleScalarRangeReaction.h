/*=========================================================================

   Program: ParaView
   Module:  pqRescaleScalarRangeReaction.h

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
#ifndef pqRescaleScalarRangeReaction_h
#define pqRescaleScalarRangeReaction_h

#include "pqReaction.h"
#include "vtkSmartPointer.h"

#include <QPointer>

class pqDataRepresentation;
class pqPipelineRepresentation;
class pqRescaleScalarRangeToCustomDialog;
class pqRescaleScalarRangeToDataOverTimeDialog;
class pqServer;
class vtkEventQtSlotConnect;
class vtkSMProxy;

/**
 * @ingroup Reactions
 * Reaction to rescale the active lookup table's range to match the active
 * representation. You can disable tracking of the active representation,
 * instead explicitly provide one using setRepresentation() by pass
 * track_active_objects as false to the constructor.
 */
class PQAPPLICATIONCOMPONENTS_EXPORT pqRescaleScalarRangeReaction : public pqReaction
{
  Q_OBJECT
  typedef pqReaction Superclass;

public:
  enum Modes
  {
    DATA,
    CUSTOM,
    TEMPORAL,
    VISIBLE
  };

  /**
   * if \c track_active_objects is false, then the reaction will not track
   * pqActiveObjects automatically.
   */
  pqRescaleScalarRangeReaction(
    QAction* parent, bool track_active_objects = true, Modes mode = DATA);
  ~pqRescaleScalarRangeReaction() override;

  /**
   * Rescale to current data range.
   *
   * @param[in] repr The data representation to use to determine the data range.
   *                 If `nullptr`, then the active representation is used, if
   *                 available.
   * @returns `true` if the operation was successful, otherwise `false`.
   */
  static bool rescaleScalarRangeToData(pqPipelineRepresentation* repr = nullptr);

  /**
   * Rescale range to a custom range.
   *
   * @param[in] repr The representation used to determine the transfer function
   *                 to change range on. If \c repr is `nullptr`, then the active
   *                 representation is used, if available.
   * @returns a pointer to the dialog if the operation was successful, otherwise `nullptr`.
   */
  static pqRescaleScalarRangeToCustomDialog* rescaleScalarRangeToCustom(
    pqPipelineRepresentation* repr = nullptr);

  /**
   * Rescale range to a custom range.
   *
   * @param[in] tfProxy The transfer function proxy to rescale the range on.
   * @param[in] separateOpacity Show controls for setting the opacity function range
   *                            separately from the color transfer function.
   *
   * @returns a pointer to the dialog if the operation was successful, otherwise `nullptr`.
   */
  static pqRescaleScalarRangeToCustomDialog* rescaleScalarRangeToCustom(
    vtkSMProxy* tfProxy, bool separateOpacity = false);

  /**
   * Rescale range to data range over time.
   *
   * @param[in] repr The data representation to use to determine the data range.
   *                 If `nullptr`, then the active representation is used, if
   *                 available.
   * @returns `true` if the operation was successful, otherwise `false`.
   */
  static pqRescaleScalarRangeToDataOverTimeDialog* rescaleScalarRangeToDataOverTime(
    pqPipelineRepresentation* repr = nullptr);

  /**
   * Rescale range to data range for data visible in the view.
   *
   * @param[in] repr The data representation to use to determine the data range.
   *                 If `nullptr`, then the active representation is used, if
   *                 available.
   * @returns `true` if the operation was successful, otherwise `false`.
   */
  static bool rescaleScalarRangeToVisible(pqPipelineRepresentation* repr = nullptr);

public Q_SLOTS: // NOLINT(readability-redundant-access-specifiers)
  /**
   * Updates the enabled state. Applications need not explicitly call this.
   */
  void updateEnableState() override;

  /**
   * Set the data representation explicitly when track_active_objects is false.
   */
  void setRepresentation(pqDataRepresentation* repr);

protected:
  /**
   * Called when the action is triggered.
   */
  void onTriggered() override;

protected Q_SLOTS: // NOLINT(readability-redundant-access-specifiers)
  virtual void onServerAdded(pqServer* server);
  virtual void onAboutToRemoveServer(pqServer* server);

private:
  Q_DISABLE_COPY(pqRescaleScalarRangeReaction)
  QPointer<pqPipelineRepresentation> Representation;
  Modes Mode;
  vtkSmartPointer<vtkEventQtSlotConnect> Connection;
};

#endif
