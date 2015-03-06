--[[
	hrender server for HIVE
--]]

package.path = package.path .. ";../../third_party/?.lua" -- for debug
JSON = require('dkjson')

function ClientType()
	return "RENDER"
end

local network = Connection()
local function connectHIVE()
	return network:Connect('ws://localhost:8080/')
end

local function mysleep(sec)
	local start = os.time()
	while os.time() - start < sec do end
end

local function sendError(err)
	local errtxt = JSON.encode({jsonrpc = "2.0", error = err});
	network:SendText(errtxt)
end

local function sendResult(ret)
	local retval = JSON.encode({jsonrpc = "2.0", result = ret});
	network:SendText(retval)
end

local function eval(src)
	local func
	local err
	local success
	local ret
	func, err = load(src)
	if func == nil or err then
		print(err)
		sendError(err)
		return
	end
	
	success, ret = pcall(func)
	print('Run:', success, 'Result:', ret)
	if success then
		if ret then
			sendResult(ret)
		end
	else -- error
		sendError(ret)
	end
end

print('HIVE renderer START');
local r = connectHIVE()
if r then
	print("Connected")
end

network:SetTimeout(10)
while true do
	local src = network:Recv()
	if src ~= '' then
		print('SRC:', src);
		if src == 'END' then
			network:Close()
			print('Exit.');
			return;
		else
			eval(src)
		end
	else
		r = network:GetState()
		if r == "CLOSED" or r == "NOT_CONNECTED" then
			mysleep(1)
			r = connectHIVE();
			if r then
				print('Reconnected')
			end
		end
	end
end
