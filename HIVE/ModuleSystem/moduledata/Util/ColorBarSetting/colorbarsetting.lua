ColorBarSetting = {}
setmetatable(ColorBarSetting, {__index = HiveBaseModule})

ColorBarSetting.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ColorBarSetting});
    return this
end

function ColorBarSetting:Do()
    self:UpdateValue()
    if self.value.colorbar == nil then
        return false;
    end
    return true
end

function ColorBarSetting:ColorBar()
    local v = self.value
    if v.colorbar ~= nil then
        v.colorbar.composite = v.composite
        v.colorbar.color = v.color
        return v.colorbar;
    end 
    return nil
end
