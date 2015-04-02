--[[
	hrender thumbnail capture for HIVE
--]]

local objpath = "bunny.obj"
local fragpath = "polygon.frag"
local jsonstr = ""
local outputpath = "out.jpg"

for i = 1, #arg do
	if i == 1 then
		objpath = arg[i]
	end
	if i == 2 then
		fragpath = arg[i]
	end
	if i == 3 then
		outputpath = arg[i]
	end
	if i == 4 then
		jsonstr = arg[i]
	end
end

JSON = require('dkjson')

-- Global Jpeg Saver
local saver = ImageSaver()

local camera = Camera()
camera:SetScreenSize(256,256)
camera:LookAt(0,0,100, 0,0,0, 0,1,0, 60)
camera:SetFilename(outputpath)

local obj = OBJLoader()
obj:Load(objpath)
local model = PolygonModel()
local meshdata = obj:MeshData()
model:Create(meshdata)
model:SetShader(fragpath)

-- set uniform values
if jsonstr then
	local data, pos, msg = JSON.decode(jsonstr)
	if not data then
		print("JSON parse error")
		print(pos)
		print(msg)
	end
	if data.uniforms then
		local uniforms = data.uniforms
		for key, value in pairs(uniforms) do
			local name, uniform, val
			for k, v in pairs(value) do
				if k == "name" then
					name = v
				end
				if k == "val" then
					val = v
				end
				if k == "uniform" then
					uniform = v
				end
			end
			if name and val then
				if uniform == "float" then
					model:SetFloat(name, val)
				end
				if uniform == "vec2" then
					model:SetVec2(name, val[1], val[2])
				end
				if uniform == "vec3" then
					model:SetVec3(name, val[1], val[2], val[3])
				end
				if uniform == "vec4" then
					model:SetVec4(name, val[1], val[2], val[3], val[4])
				end
			end
		end
	end
end

render {camera, model}

