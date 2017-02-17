BufferSet = {}
setmetatable(BufferSet, {__index = HiveBaseModule})

local JSON = require('dkjson')

BufferSet.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=BufferSet})
    return this
end

function BufferSet:SendIDs()
    local imageIDs = getBufferDataNames()
    
    if network then
        local data = {}
        data["JSONRPC"] = "2.0"
        data["method"] = "luaData"
        data["to"] = targetClientId
        data["id"] = 0
        data["param"] = {}
        data["param"]["name"] = "BufferSet"
        data["param"]["type"] = "bufferids"
        data["param"]["images"] = imageIDs
        network:SendJSON( JSON.encode(data) );
    end
end

function BufferSet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local buffer = v.bufferdata
    local selected = v.selected
    
    if buffer ~= nil and id ~= "" then
        setBufferData(id, buffer);
        self:SendIDs();
        print("buffer saved")
    end
    
    if v.delete == true then
        selected = string.gsub(selected, "'", "\"")
        local selectList = JSON.decode(selected)
        if selectList ~= nil then
            if #selectList.selected > 0 then
                for i, buffer in ipairs(selectList.selected) do
                    deleteBufferData(buffer);
                    print("BufferSet:" .. buffer .. " deleted")
                end
            end
        end
        self:SendIDs();
    end
    if v.deleteall == true then
        clearBufferData();
        self:SendIDs();
        print("BufferSet: delete all the buffers")
    end
    
    return true
end
