LoadPDMMx1 = {}
setmetatable(LoadPDMMx1, {__index = HiveBaseModule})

LoadPDMMx1.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('PdmLoader')()
    setmetatable(this, {__index=LoadPDMMx1})
    return this
end

function LoadPDMMx1:Do()
    self:UpdateValue()
	if self.value.filepath ~= nil and self.value.filepath ~= "" then
       return self.loader:LoadMx1(self.value.filepath, self.value.timestep, nil)
	else
		return false
	end
end

function LoadPDMMx1:PointData()
   return self.loader:PointData(self.value.container, self.value.radius)
end

function LoadPDMMx1:ExtraData(index)
    local name = self.value.extraname[index + 1]
    return self.loader:ExtraData(name)
end
