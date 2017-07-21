VaryingExtraData = {}
setmetatable(VaryingExtraData, {__index = HiveBaseModule})

VaryingExtraData.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=VaryingExtraData})
    return this
end

function VaryingExtraData:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function VaryingExtraData:varying()
    local v = self.value
    return { name = v.name, type = "varying", value = v.extradata}
end
