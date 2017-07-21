BufferGet = {}
setmetatable(BufferGet, {__index = HiveBaseModule})

BufferGet.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=BufferGet})
    this.buffer_data = nil
    return this
end

function BufferGet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local buffer = getBufferData(id);
    if buffer ~= nil then
        self.buffer_data = buffer;
    end
    return true
end

function BufferGet:bufferdata()
    return self.buffer_data
end
