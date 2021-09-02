
"use strict";

let SetGoal = require('./SetGoal.js')
let ListLabels = require('./ListLabels.js')
let ResetPose = require('./ResetPose.js')
let GetNodeData = require('./GetNodeData.js')
let GetMap = require('./GetMap.js')
let GetMap2 = require('./GetMap2.js')
let GetNodesInRadius = require('./GetNodesInRadius.js')
let PublishMap = require('./PublishMap.js')
let AddLink = require('./AddLink.js')
let SetLabel = require('./SetLabel.js')
let GetPlan = require('./GetPlan.js')
let LoadDatabase = require('./LoadDatabase.js')

module.exports = {
  SetGoal: SetGoal,
  ListLabels: ListLabels,
  ResetPose: ResetPose,
  GetNodeData: GetNodeData,
  GetMap: GetMap,
  GetMap2: GetMap2,
  GetNodesInRadius: GetNodesInRadius,
  PublishMap: PublishMap,
  AddLink: AddLink,
  SetLabel: SetLabel,
  GetPlan: GetPlan,
  LoadDatabase: LoadDatabase,
};
