
local function getObjectList()
	local lst = {}
	for i,v in pairs(HIVE_ObjectTable) do
		print(i, v)
		local robj = v
		local info = {}
		info.filename  = HIVE_DataTable[i]
		info.translate = robj:GetTranslate()
		info.rotate    = robj:GetRotate()
		info.scale     = robj:GetScale()
		info.vec4      = robj:GetVec4Table()
		info.vec3      = robj:GetVec3Table()
		info.vec2      = robj:GetVec2Table()
		info.float     = robj:GetFloatTable()
		info.rgbatex   = robj:GetTextureTable();
		if robj:GetType() == 'CAMERA' then
			info.position = robj:GetPosition()
			info.target = robj:GetTarget()
			info.up = robj:GetUp()
			info.fov = robj:GetFov()
			info.screensize = robj:GetScreenSize()
			info.clearcolor = robj:GetClearColor()
			info.outputfile = robj:GetOutputFile()
		else
			info.shader = robj:GetShader()
		end
		lst[#lst + 1] = {name = i, type = robj:GetType(), info = info}
	end
	return lst
end

local function getObjectTimeline()
	return HIVE_ObjectTimeline
end

local function StoreObjectTimeline(timeline_json)
	local timeline = JSON.decode(timeline_json)
	HIVE_ObjectTimeline = timeline
end

local function updateInfo()
	if not network then
		return
	end
	local objlst = getObjectList()
	local objtimeline = getObjectTimeline()
	local data = {
		JSONRPC = "2.0",
		method  = "updateInfo",
		param   = JSON.encode({
			objectlist = objlst,
			objecttimeline = objtimeline
		}),
		to = targetClientId,
		id = 0
	}
	local json = JSON.encode(data)
	network:SendText(json)
end

local function UpdateSceneInformation()
	updateInfo();
	return 'UpdateSceneInformation: send updateInfo';
end

local function ClearObjects()
	HIVE_ObjectTable = {}
	HIVE_ObjectTimeline = {}
	clearCache()
	collectgarbage('collect')
	print('MEMORY=', collectgarbage('count') .. '[KB]')
	return 'ClearObject'
end

local function CreateCamera(name)
	local camera = Camera()
	camera:SetScreenSize(512, 512)
	camera:LookAt(
		0,0,300,
		0,0,0,
		0,1,0,
		60
	)
	camera:SetFilename(name .. '.jpg')
	HIVE_ObjectTable[name] = camera
	updateInfo()
	return 'CreateCamera:' .. name
end

local function NewScene()
	ClearObjects()
	CreateCamera('view')
	updateInfo();
	return 'NewScene!'
end

-- TEST METHOD
local function CameraPos(name, camerapos_x, camerapos_y, camerapos_z)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera:LookAt(
		camerapos_x, camerapos_y, camerapos_z,
		0,0,0,
		0,1,0,
		60
	)
end

local function CameraLookat(name, pos_x, pos_y, pos_z, tar_x, tar_y, tar_z, up_x, up_y, up_z, fov)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera:LookAt(
		pos_x, pos_y, pos_z,
		tar_x, tar_y, tar_z,
		up_x,  up_y,  up_z,
		fov
	)
end

local function CameraScreenSize(name, width, height)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera:SetScreenSize(width, height)
	return 'ScreenSize:' .. name
end

local function CameraClearColor(name, red, green, blue, alpha)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera:ClearColor(red, green, blue, alpha)
	return 'ClearColor:' .. name
end

local function CameraOutputFilename(name, filename)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera:SetFilename(filename)
	return 'SetFilename:' .. name
end

local function GetVolumeAnalyzerData(name, min, max)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	local analyzer = require('Analyzer').VolumeAnalyzer()
	analyzer:Execute(model:GetVolume())
	local histgram;
	local volMin = analyzer:MinX()
	local volMax = analyzer:MaxX()
	histgram = analyzer:GetHistgram();
	--else
	--	histgram = analyzer:GetHistgramInRange(model, min, max);
	--end
	if min == nil or max == nil then
		return {name=name, defaultMin=volMin, defaultMax=volMax, min=volMin, max=volMax, histgram=histgram}
	else
		return {name=name, defaultMin=volMin, defaultMax=volMax, min=min, max=max, histgram=histgram}
	end
end

local function SetModelShader(name, shaderpath)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetShader(shaderpath)
	return 'SetModelShader:' .. name .. '<=' .. shaderpath
end

local function SetModelTranslation(name, trans_x, trans_y, trans_z, rot_x, rot_y, rot_z, scale_x, scale_y, scale_z)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetTranslate(trans_x, trans_y, trans_z)
	model:SetRotate(rot_x, rot_y, rot_z)
	model:SetScale(scale_x, scale_y, scale_z)
	return 'SetModelTranslation:' .. name
end

local function SetModelUniformVec4(name, vname, x, y, z, w)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetVec4(vname, x, y, z, w)
	return 'SetModelUniformVec4:' .. name
end
local function SetModelUniformVec3(name, vname, x, y, z)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetVec3(vname, x, y, z)
	return 'SetModelUniformVec3:' .. name
end
local function SetModelUniformVec2(name, vname, x, y)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetVec2(vname, x, y)
	return 'SetModelUniformVec2:' .. name
end
local function SetModelUniformFloat(name, vname, val)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	model:SetFloat(vname, val)
	return 'SetModelUniformFloat:' .. name
end

local function SetModelUniformTex(name, vname, width, height, rgba)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end

	local gentex = GenTexture()
	gentex:Create2D(rgba, 1, width, height);
	model:SetTexture(vname, gentex:ImageData())
	return 'SetModelUniformTex:' .. name
end

local function SetColorMapTex(texname, width, height, rgba)
	local gentex = GenTexture()
	gentex:Create2D(rgba, 1, width, height);
	HIVE_TextureTable[texname] = gentex;
	return 'SetColorMapTex:' .. texname
end

local function ApplyColorMapTex(name, texname, uniformName)
	local model = HIVE_ObjectTable[name]
	if model == nil then return 'Not found Model:' .. name end
	local texture = HIVE_TextureTable[texname]
	if texture == nil then return 'Not found Texture:' .. texname end

	model:SetTexture(uniformName, texture:ImageData())
	return 'ApplyColorMapTex:' .. name
end

local function DeleteObject(name)
	local obj = HIVE_ObjectTable[name]
	if obj == nil then return 'Not found object' end
	HIVE_ObjectTable[name] = nil
	HIVE_ObjectTimeline[name] = nil
	HIVE_DataTable[name] = nil
	updateInfo();
end

local function LoadSPH(name, filename, shader)
	local sph = require("SphLoader")()
	local ret = sph:Load(filename)
	if ret then
	    local volumemodel = VolumeModel()
	    local volume = sph:VolumeData()
	    volumemodel:Create(volume)
	    volumemodel:SetShader(shader)
		HIVE_ObjectTable[name] = volumemodel
		HIVE_DataTable[name] = filename
		updateInfo()
	end
	return 'LoadPDB:' .. tostring(ret)
end

local function LoadOBJ(name, filename, shader)
	local obj = require("ObjLoader")()
	local ret = obj:Load(filename)
	if ret then
		local model = PolygonModel()
		local meshdata = obj:MeshData()
		model:Create(meshdata)
		model:SetShader(shader)
		HIVE_ObjectTable[name] = model
		HIVE_DataTable[name] = filename
		updateInfo()
	end
	return 'LoadOBJ:' .. tostring(ret)
end


local function LoadSTL(name, filename, shader)
	local stl = require("StlLoader")()
	local ret = stl:Load(filename)
	if ret then
		local model = PolygonModel()
		local meshdata = stl:MeshData()
		model:Create(meshdata)
		model:SetShader(shader)
		HIVE_ObjectTable[name] = model
		HIVE_DataTable[name] = filename
		updateInfo()
	end
	return 'LoadSTL:' .. tostring(ret)
end

local function LoadPDB(name, filename, shader)
	local pdb = require("PdbLoader")()
	local ret = pdb:Load(filename)
	if ret then
	    local ballmodel = PointModel()
	    local balldata = pdb:BallData()
	    ballmodel:Create(balldata)
	    ballmodel:SetShader(shader)
	    local stickmodel = LineModel()
	    local stickdata = pdb:StickData()
	    stickmodel:Create(stickdata)
	    stickmodel:SetLineWidth(0.20);
	    stickmodel:SetShader(shader)
		HIVE_ObjectTable[name .. '_ball']  = ballmodel
		HIVE_ObjectTable[name .. '_stick'] = stickmodel
		HIVE_DataTable[name .. '_ball'] = filename -- TODO
		HIVE_DataTable[name .. '_stick'] = filename -- TODO
		updateInfo()
	end
	return 'LoadPDB:' .. tostring(ret)
end
local function RenderCamera(w, h, cameraname, imagemode, ipcmode)
	--print('RenderCamera', w, h, cameraname, imagemode, ipcmode)
	local starttm = os.clock()
	local camera = HIVE_ObjectTable[cameraname]
	if camera == nil then return "No Camera" end
	local oldscreensize = camera:GetScreenSize()
	local oldfilename   = camera:GetOutputFile();
	local renderList = {camera}
	for i,v in pairs(HIVE_ObjectTable) do
		--print ("OBJECT = ", i, v)
	    if v:GetType() ~= "CAMERA" then
		        renderList[#renderList + 1] = v
	    end
	end
	--print('RenderObject = ', #renderList)
	local prefetchNextEvent = HIVE_fetchEvent(0)
	if prefetchNextEvent == false then -- false is any queued
		--print('Skip rendering')
		return 'Skip rendering'
	end

	-- set
	camera:SetScreenSize(w, h)
	camera:SetFilename('')
	local r = render(renderList, HIVE_fetchEvent)
	-- restore
	camera:SetScreenSize(oldscreensize[1], oldscreensize[2])
	camera:SetFilename(oldfilename)

	local rendertm = os.clock()
	--print('Render Ret = ', r)

	-- make metabin

	local sendmode = "jpg"
	if imagemode ~= nil then
		sendmode = imagemode
	end

	local json = [[{
		"JSONRPC": "2.0",
		"method" : "renderedImage",
		"param" : {
		 	"type": "]] .. sendmode .. [[",
		    "width" : ]] .. w .. [[,
			"height" : ]] .. h .. [[,
		 	"canceled": ]] .. tostring(HIVE_isRenderCanceled) .. [[
		},
		"to": "]] .. targetClientId .. [[",
		"id": 0
	}]]
	local createtm = 0
	if HIVE_metabin then
		if (sendmode == "jpg") then
			-- save jpg
			local imageBuffer = HIVE_ImageSaver:SaveMemory(1, camera:GetImageBuffer())
			local imageBufferSize = HIVE_ImageSaver:MemorySize()
			savetm = os.clock()
			HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
			createtm = os.clock()
		elseif (sendmode == "raw") then
			savetm = rendertm -- no save
			local img = camera:GetImageBuffer()
			HIVE_metabin:Create(json, img:GetBuffer(), img:GetSize())
			createtm = os.clock()
		else
			print('[Error] Unsupported data type sendmode =>', sendmode)
		end

		-- 	send through websocket
		if ipcmode == true and network_ipc then
			network_ipc:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
		else
			network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
		end
	end
	local sendtm = os.clock()
	--print("render=", rendertm-starttm, "save=", savetm-rendertm,"createmeta=", createtm-savetm, "send=", sendtm-createtm, "all=", sendtm-starttm)


	collectgarbage('collect') -- NEED for GC
	return {objectnum=tostring(r), width=w, height=h}
end

return {
	NewScene = NewScene,
	ClearObjects = ClearObjects,
	CreateCamera = CreateCamera,
	CameraPos = CameraPos,
	CameraScreenSize = CameraScreenSize,
	CameraClearColor = CameraClearColor,
	CameraOutputFilename = CameraOutputFilename,
	SetModelShader = SetModelShader,
	SetModelTranslation = SetModelTranslation,
	SetModelUniformVec4 = SetModelUniformVec4,
	SetModelUniformVec3 = SetModelUniformVec3,
	SetModelUniformVec2 = SetModelUniformVec2,
	SetModelUniformFloat = SetModelUniformFloat,
	SetModelUniformTex = SetModelUniformTex,
	SetColorMapTex = SetColorMapTex,
	ApplyColorMapTex = ApplyColorMapTex,
	DeleteObject = DeleteObject,
	LoadSPH = LoadSPH,
	LoadOBJ = LoadOBJ,
	LoadSTL = LoadSTL,
	LoadPDB = LoadPDB,
	UpdateSceneInformation = UpdateSceneInformation,
	RenderCamera = RenderCamera,
	CameraLookat = CameraLookat,
	GetVolumeAnalyzerData = GetVolumeAnalyzerData,
	StoreObjectTimeline = StoreObjectTimeline
}
