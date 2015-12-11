--[[
	hrender server for HIVE
--]]

arg = {...}

hcmd = require('HiveCommand')

if hcmd == nil then
	print('[Error] Not found HiveCommand.lua')
else
	print('----[HiveCommand]----')
	for i,v in pairs(hcmd) do
		print('hcmd.' .. i)
	end
	print('---------------------')
end

local connectAddress = 'ws://localhost:8080/' 
local ipcAddress = 'ipc:///tmp/HIVE_server_ipc'

if arg[1] and arg[1]:sub(1,5) == 'ws://' then
	connectAddress = arg[1]
end

if arg[2] and arg[2]:sub(1,5) == 'ipc://' then
	ipcAddress = arg[2]
end

print('HIVE renderer START. Connect to', connectAddress);


local Log = function() end
-- for Debug
--Log = print

package.path = package.path .. ";../../third_party/?.lua" -- for debug
JSON = require('dkjson')

-- Global Weboket Connection
network = require("Network").Connection()
network:SetTimeout(100)

network_ipc = require("Network").Connection()
local ipcr = network_ipc:Connect(ipcAddress)


-- Global Jpeg Saver
HIVE_ImageSaver = ImageSaver()

-- Global Metabin
HIVE_metabin = require("Network").MetaBinary()


local defaultCamera = Camera() -- 'view'
defaultCamera:SetScreenSize(256,256)
defaultCamera:SetFilename('view.jpg')
defaultCamera:LookAt(0,0,300, 0,0,0, 0,1,0, 60)
HIVE_ObjectTable = {view=defaultCamera} -- Global: All Object List
HIVE_ObjectTimeline = {}
HIVE_DataTable   = {} -- Global: Data List

local function mysleep(sec)
	local start = os.time()
	while os.time() - start < sec do end
end

--[[
local function sendMasterError(err)
	local errtxt = JSON.encode({jsonrpc = "2.0", error = err, to = 'master'});
	network:SendText(errtxt)
end

local function sendMasterResult(ret)
	local retval = JSON.encode({jsonrpc = "2.0", result = ret});
	network:SendText(retval)
end
--]]

local function sendClientError(err, id)
	local errtxt = JSON.encode({jsonrpc = "2.0", error = err, to = 'client', id = id});
	network:SendText(errtxt)
end

local function sendClientResult(ret, id)
	local retval = JSON.encode({jsonrpc = "2.0", result = ret, to = 'client', id = id});
	network:SendText(retval)
end

local function sendMasterMethod(method, param)
	local retval = JSON.encode({jsonrpc = "2.0", method = method, param = param, to = 'master'});
	network:SendText(retval)
end


local function connectHIVE()
	local r = network:Connect(connectAddress)
	if r then
		sendMasterMethod('register', {mode = 'renderer'})
	end
	return r
end

local function eval(src)
	if src == nil then
		return nik, '[Error] Script src == nil'
	end
	local func
	local err
	local success
	local ret
	func, err = load(src)
	if func == nil or err then
		return nil, err
	end
	
	success, ret = pcall(func)
	
	if success then
		if ret then
			return ret, nil
		end
	else -- error
		return nil, ret
	end
end

function renderMethod(method, param, id)
	local ret
	local err
	local jsonSuccess
	local rjson
	Log('============= RUN ============')
	Log('[DEBUG] method = ', method)
	if method == 'runscript' then
		Log('------------------------------')
		ret, err = eval(param.script)
		Log('------------------------------')
		Log('[DEBUG] eval -> Return=', ret, 'Err=', err)
		if err then
			sendClientError(err, id)
		else
			jsonSuccess, rjson = pcall(function ()
				return JSON.encode(ret)
			end)
			if jsonSuccess then
				--sendClientResult(rjson,id); --- TOOOOOOOOOOOOOOOOOSLOWWWWWWW!!!!!! / TODO
			else -- json error
				sendClientError(rjson, id)
			end
		end
	end
	Log('==============================')
end

HIVE_nextEvent = ''
HIVE_isRenderCanceled = false
function HIVE_fetchEvent(progress)
	HIVE_nextEvent = network:Recv()
	--print('Rendering: ', progress .. '%', HIVE_nextEvent)
	if HIVE_nextEvent ~= '' then
		HIVE_isRenderCanceled = true
		return false
	else
		return true
	end
end

--function HIVE_fetchEvent(progress)
	-- for GL
--end

function mainloop()
	local src = ''
	local data
	local err
	while true do
		if HIVE_nextEvent ~= '' then
			src = HIVE_nextEvent
			HIVE_nextEvent = ''
		else
			src = network:Recv()
		end
		if src ~= '' then
			Log('[DEBUG] SRC:', src);
			data = JSON.decode(src)
			if data.method == 'end' then
				network:Close()
				print('Exit.');
				return;
			else
				HIVE_isRenderCanceled = false
				renderMethod(data.method, data.param, data.id)
			end
		else
			r = network:GetState()
			-- Reconnection
			if r == "CLOSED" or r == "NOT_CONNECTED" then
				mysleep(1)
				r = connectHIVE();
				if r then
					Log('[Connection] Reconnected')
					-- For debug force reload
					package.loaded['HiveCommand'] = nil
					hcmd = require('HiveCommand')
				end
			end		
		end
	end
end

----------------------------------

local r = connectHIVE()
if r then
	Log("Connected")
end
mainloop()
