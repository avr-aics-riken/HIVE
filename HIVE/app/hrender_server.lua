--[[
	hrender server for HIVE
--]]

package.path = package.path .. ";../../third_party/?.lua" -- for debug
JSON = require('dkjson')

local network = Connection()
network:SetTimeout(100)


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
	local r = network:Connect('ws://localhost:8080/')
	if r then
		sendMasterMethod('register', {mode = 'renderer'})
	end
	return r
end

local function eval(src)
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
	print('[DEBUG] method = ', method)
	if method == 'runscript' then
		print('----------- RUN --------------')
		ret, err = eval(param.script)
		print('------------------------------')
		print('[DEBUG] eval -> Return=', ret, 'Err=', err)
		if err then
			sendClientError(err, id)
		else
			jsonSuccess, rjson = pcall(function ()
				return JSON.encode(ret)
			end)
			if jsonSuccess then
				sendClientResult(rjson,id);
			else -- json error
				sendClientError(rjson, id)
			end
		end
	end
end

function mainloop()
	local src
	local data
	local err
	while true do
		src = network:Recv()
		if src ~= '' then
			print('SRC:', src);
			data = JSON.decode(src)
			if data.method == 'end' then
				network:Close()
				print('Exit.');
				return;
			else
				renderMethod(data.method, data.param, data.id)
			end
		else
			r = network:GetState()
			-- Reconnection
			if r == "CLOSED" or r == "NOT_CONNECTED" then
				mysleep(1)
				r = connectHIVE();
				if r then
					print('[Connection] Reconnected')
				end
			end
		end
	end
end

----------------------------------

print('HIVE renderer START');
local r = connectHIVE()
if r then
	print("Connected")
end
mainloop()
