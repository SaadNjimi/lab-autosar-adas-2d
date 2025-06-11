"""
--------------------------------------------------------------------------------
File:        EcuInstance.py

Description: Generated script to build the EcuInstance V-ECU for VEOS.

Tip/Remarks: You may start this script from SystemDesk or directly.

Limitations: -

Template:    1.2

Generated:   6/11/2025 10:13 AM

             dSPACE GmbH shall not be liable for errors contained herein or
             direct, indirect, special, incidental, or consequential damages
             in connection with the furnishing, performance, or use of this
             file.
             Brand names or product names are trademarks or registered
             trademarks of their respective companies or organizations.

Copyright (c) 2021 by dSPACE GmbH, Paderborn, Germany
  All Rights Reserved.
--------------------------------------------------------------------------------
"""


from win32com.client import Dispatch
import sys
import os
import subprocess
import argparse

def getAbsPathFromRelativeToMe(path):
    """Returns an absolute path using the script file location as base path if the path is not absolute already."""
    if os.path.isabs(path):
        return path

    mydir, _ = os.path.split(os.path.realpath(__file__))
    return os.path.normpath(os.path.join(mydir, path))

# Options

# This script uses the active project or the following file if no project is loaded in SystemDesk.
projectFile = r'C:\Users\Ase-Lab\Desktop\ASE_Lab_Unit_4_Template\ASE_Lab_Unit_4_Template\unit4.sdp'
projectFile = getAbsPathFromRelativeToMe(projectFile)

# The name of the V-ECU to be build
vEcuName = 'EcuInstance'

# The V-ECU is exported into this file.
containerFile = r'.\EcuInstance.vecu'
containerFile = getAbsPathFromRelativeToMe(containerFile)

# Path to the VEOS build tool
veosBuildTool = r'C:\Program Files\dSPACE VEOS 5.4\bin\veos-build.exe'

# The output file as string, specify this to create a simulation with more than one V-ECU
outputFile = r'.\EcuInstance\Build\EcuInstance.osa'
outputFile = getAbsPathFromRelativeToMe(outputFile)

# The directory that is used to store temporary files during build.
# Specify None to let VEOS chose the location.
buildDirectory = r'.\EcuInstance\Build'
buildDirectory = getAbsPathFromRelativeToMe(buildDirectory)

# Path of the build logfile. If a relative path is specified the resulting path is determined relative to the buildDirectory.
logFilePath = 'EcuInstance.Build.log'

# The target board and compiler as string, possible values are 'HostPC32/GCC', 'HostPC32/MSVC', 'HostPC64/GCC', 'HostPC64/MSVC', 'HostPCLinux64/GCC'.
# Specify None to use the default build target of VEOS.
target = None

# The ethernet port to be used for calibration and measurement access.
# Specify None to use the default port of VEOS.
xcpServicePort = None

# A string that is passed as argument to the C compiler. Specify None to omit this option.
compilerOptions = None

# A string that is passed as argument to the C++ compiler. Specify None to omit this option.
cppCompilerOptions = None

# A list used to specify preprocessor defines.
preprocessorDefines = [  ]

# A list of files to be included in the V-ECU.
additionalCodeFiles = [  ]

# The code coverage level, possible values are 'None', 'FunctionCoverage', 'DecisionCoverage', 'MultiConditionCoverage', 'TimingExclusive', 'TimingInclusive'
codeCoverageLevel = None

# A flag used to configure the compiler to output all warnings.
showAllWarnings = False

# The build configuration, possible values are 'Debug' and 'Release'. Specify None to use the default value of VEOS, i.e. 'Debug'.
configuration = None

# A string used to specify the author of the output file.
osaAuthor = None


# Constants
scriptName = 'BuildScript ' + os.path.basename(__file__)

def getSystemDesk():
    """Open COM connection to SystemDesk"""
    sd = Dispatch("SystemDesk.Application")
    if (sd == None):
        raise Exception("Could not find SystemDesk.")
    return sd

def raiseException(sd, msg):
    """Submits an error message in SystemDesk and raises an exception with the same message"""
    sd.SubmitErrorMessage(scriptName, msg)
    raise Exception(msg)

def reportInfo(sd, msg):
    """Submits an info message in SystemDesk and prints the same message"""
    sd.SubmitInfoMessage(scriptName, msg)
    print(msg)

def reportInfoWithUrl(sd, msg, url):
    """Submits an info message with URL in SystemDesk and prints the same message"""
    sd.SubmitInfoMessageWithUrl(scriptName, msg, url)
    print(msg + ": " + url)

def reportError(sd, msg):
    """Submits an error message in SystemDesk and prints the same message"""
    sd.SubmitErrorMessage(scriptName, msg)
    print(msg, file=sys.stderr)

def getVEcu(sd):
    """Get V-ECU to be built"""
    if (sd.ActiveProject == None):
        if not os.path.exists(projectFile):
            raiseException(sd, "Cannot find the project file at " + projectFile + ".")

        sd.OpenProject(projectFile)
        if (sd.ActiveProject == None):
            raiseException(sd, 'Failed to open the required project: %s' % projectFile)
    vEcu = sd.ActiveProject.VEcus.Item(vEcuName)
    if (vEcu == None) or (vEcu.ElementType != 'IClassicVEcu' and vEcu.ElementType != 'IAdaptiveVEcu'):
        raiseException(sd, 'Failed to get the required V-ECU: %s' % vEcuName)
    return vEcu

def exportContainer(sd, vEcu):
    """Export V-ECU implementation"""
    reportInfo(sd, "Exporting the V-ECU implementation '" + vEcu.Name + "'...")
    container = vEcu.ExportContainer(os.path.normpath(containerFile))
    if (container == None):
        raiseException(sd, 'V-ECU implementation export failed.')
    reportInfoWithUrl(sd, "V-ECU implementation successfully exported", container)
    return container

def callVeosBuild(sd, vEcu, container):
    """Build V-ECU with the VEOS build tool"""
    reportInfo(sd, "Starting a VEOS build of the exported V-ECU implementation...")

    if not os.path.exists(veosBuildTool):
        raiseException(sd, "Cannot find the VEOS build tool at: " + veosBuildTool)
    else:
        reportInfoWithUrl(sd, "Using the VEOS build tool at", veosBuildTool)

    buildAction = 'classic-vecu'
    if vEcu.ElementType == 'IAdaptiveVEcu':
        buildAction = 'adaptive-vecu'

    arguments = str.format('"{}" {} "{}"', veosBuildTool, buildAction, container)

    if outputFile != None:
        arguments += ' -o "' + outputFile + '"'
        reportInfoWithUrl(sd, "Writing the simulation system to", outputFile)
    if buildDirectory != None:
        arguments += ' --build-directory "' + buildDirectory + '"'
    if logFilePath != None:
        if os.path.isabs(logFilePath):
            absoluteLogFilePath = logFilePath
        else:
            absoluteLogFilePath = os.path.normpath(os.path.join(buildDirectory, logFilePath))
        arguments += ' --log-file-path "' + absoluteLogFilePath + '"'
        reportInfoWithUrl(sd, "Writing the build log to", absoluteLogFilePath)

    if vEcu.ElementType != 'IAdaptiveVEcu':
        if target != None:
            arguments += ' -t ' + target
        if configuration != None:
            arguments += ' --configuration ' + configuration
        if compilerOptions != None:
            arguments += ' --compiler-options="' + compilerOptions + '"'
        if cppCompilerOptions != None:
            arguments += ' --cpp-compiler-options="' + cppCompilerOptions + '"'
        if len(preprocessorDefines) > 0:
            arguments += ' --preprocessor-defines="' + ' '.join(preprocessorDefines) + '"'
        if len(additionalCodeFiles) > 0:
            arguments += ' --additional-code-files "' + '" "'.join(additionalCodeFiles) + '"'
        if xcpServicePort != None:
            arguments += ' --xcp-service-port {0}'.format(xcpServicePort)

    if codeCoverageLevel != None:
        arguments += ' --code-coverage-level ' + codeCoverageLevel
    if showAllWarnings:
        arguments += ' --show-all-warnings'
    if osaAuthor:
        arguments += ' --osa-author "' + osaAuthor + '"'

    return subprocess.run(arguments)

def build():
    """Mаin function"""
    sd = getSystemDesk()
    reportInfoWithUrl(sd, 'Starting execution of build script', __file__)
    vEcu = getVEcu(sd)
    container = exportContainer(sd, vEcu)
    buildResult = callVeosBuild(sd, vEcu, container)
    returncode = 0
    if buildResult != None:
        returncode = buildResult.returncode
    else:
        returncode = -1

    if returncode == 0:
        reportInfo(sd, "VEOS Build finished successfully.")
    else:
        reportError(sd, "VEOS Build finished with errors. See the script output for details.")

    return returncode

def setOrReplaceDefine(define):
    name = define.split('=', 1)[0]
    for i in range(len(preprocessorDefines)):
        if preprocessorDefines[i] == name or preprocessorDefines[i].startswith(name + '='):
            preprocessorDefines[i] = define
            break
    else:
        preprocessorDefines.append(define)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Start build of V-ECU ' + vEcuName, allow_abbrev=False)
    parser.add_argument('-o', '--outputFile', help='Name of the OSA file.')
    parser.add_argument('--buildDirectory', help='The directory that shall be used to store build artefacts.')
    parser.add_argument('--logFilePath', help='Path of the build logfile.')
    parser.add_argument('--compilerOptions', help='Additional C compiler options.')
    parser.add_argument('--cppCompilerOptions', help='Additional C++ compiler options.')
    parser.add_argument('--preprocessorDefines', nargs='+', help='Preprocessor defines.')
    parser.add_argument('--additionalCodeFiles', nargs='+', help='Additional code files.')
    parser.add_argument('--showAllWarnings', action='store_true', default=None, help='Show all compiler warnings.')
    parser.add_argument('--configuration', choices=['Debug', 'Release'], help='Specifies the build configuration.')
    parser.add_argument('--osaAuthor', help='Name of the OSA file author')

    args = parser.parse_args()

    if args.outputFile != None:
        outputFile = os.path.abspath(args.outputFile)

    if args.buildDirectory != None:
        buildDirectory = os.path.abspath(args.buildDirectory)

    if args.logFilePath != None:
        logFilePath = args.logFilePath

    if args.compilerOptions != None:
        compilerOptions += ' ' + args.compilerOptions.strip("'")

    if args.cppCompilerOptions != None:
        cppCompilerOptions += ' ' + args.cppCompilerOptions.strip("'")

    if args.preprocessorDefines != None:
        for define in args.preprocessorDefines:
            setOrReplaceDefine(define)

    if args.additionalCodeFiles != None:
        additionalCodeFiles.extend(args.additionalCodeFiles)

    if args.showAllWarnings != None:
        showAllWarnings = args.showAllWarnings

    if args.configuration != None:
        configuration = args.configuration

    if args.osaAuthor != None:
        osaAuthor = args.osaAuthor

    sys.exit(build())
