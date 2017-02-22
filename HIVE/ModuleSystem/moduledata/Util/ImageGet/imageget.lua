ImageGet = {}
setmetatable(ImageGet, {__index = HiveBaseModule})

ImageGet.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ImageGet})
    this.buffer_data = nil
    return this
end

function ImageGet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local buffer = getBufferData(id);
    if buffer ~= nil then
        self.buffer_data = buffer;
    end
    return true
end

function ImageGet:image()
    return self.buffer_data
end
