// This Groovy script is actually
// a workflow specification for running the MC production
// files.

node("tev") {
  checkout([$class: 'SubversionSCM', locations: [[credentialsId: '90efac96-2fa5-41df-a4f6-d22871b7056d', depthOption: 'infinity', ignoreExternalsOption: true, local: '.', remote: 'https://svn.cern.ch/reps/atlasphys-exo/Physics/Exotic/UEH/DisplacedJets/Run2/MCValidation']], workspaceUpdater: [$class: 'UpdateUpdater']])

  def runFile = sh("ls mcjo/*${MCRUN}*.py")
  def stub = "${runFile}" =~ "mcjo/(.+).py"
  echo stub[0][0]
  //echo stub[0][1]
  stub = null
}

