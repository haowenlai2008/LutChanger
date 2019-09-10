import os
import os.path
import json
# this folder is custom
rootdir="./wideFilter"
picPaths = []
for parent,dirnames,filenames in os.walk(rootdir):
	#case 2
    for filename in filenames:
        fullPath = os.path.join(parent,filename)
        print("filename with full path:"+ fullPath)
        if filename.endswith(".json"):
            data = []
            with open(fullPath, 'r') as f:
                data = json.load(f)
            if (abs(data["effects"][0]["uniformParams"]["uQuadDim"][0] - 8.0) < 0.5):
                picPath = parent + '/' + data["effects"][0]["uniformTextureData"]["inputImageTexture2"]
                print("512*512" + picPath)
                data["effects"][0]["uniformParams"]["uQuadDim"][0] = 4.0
                data["effects"][0]["uniformParams"]["uQuadPixel"][0] = 16.0
                picPaths.append(picPath)
                
#                with open(fullPath, 'w') as f:
#                    json.dump(data, f)

cmd = 'cmd.exe /k 3_Texture.exe'
for p in picPaths:
    cmd = cmd + ' ' + p
os.system(cmd)