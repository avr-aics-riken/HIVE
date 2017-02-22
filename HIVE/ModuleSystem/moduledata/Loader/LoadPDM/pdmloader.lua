LoadPDM = {}
setmetatable(LoadPDM, {__index = HiveBaseModule})

LoadPDM.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('PdmLoader')()
    setmetatable(this, {__index=LoadPDM})
    return this
end

function LoadPDM:Do()
    self:UpdateValue()
	if self.value.filepath ~= nil and self.value.filepath ~= "" then
        return self.loader:Load(self.value.filepath, self.value.timestep)
	else
		return false
	end
end

function LoadPDM:PointData()
   return self.loader:PointData(self.value.container, self.value.radius)
end

function LoadPDM:ExtraData(index)
    local name = self.value.extraname[index + 1]
    return self.loader:ExtraData(name)
end
