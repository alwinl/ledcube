import("env")
import subprocess

def before_build(source, target, env):
	# Build the host program
	subprocess.call(["make_frames", "build host program"])

env.AddPreAction("buildprog", before_build)
