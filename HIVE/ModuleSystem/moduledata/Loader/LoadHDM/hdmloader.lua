

function LoadHDM(property)
	local hdm = LoadModule("HdmLoader")
	hdm:Init(property.cellidpath, property.datapath)
	-- field name, field type, components(1=scalar, 3=vector), stepNo
	local volumedata = hdm:LoadField(self.value.fieldname, self.value.fieldtype, 1, 0)
	function sparsevolume()
		return volumedata
	end
	return {sparsevolume=sparsevolume}
end


LoadHDM = {}
setmetatable(LoadHDM, {__index = HiveBaseModule})

LoadHDM.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = LoadModule('HdmLoader')
    setmetatable(this, {__index=LoadHDM})
    return this
end

function LoadHDM:Do()
	self.loader:Init(self.value.cellidpath, self.value.datapath)
    self:UpdateValue()
    self.volumedata = self.loader:LoadField(self.value.filepath)
	if self.volumedata then
		return true
	else
		return false
	end
end

function LoadHDM:sparsevolume()
    return this.volumedata
end

function LoadHDM:Size()
    local size = {self.loader:Width(), self.loader:Height(), self.loader:Depth()}
    return size
end

function LoadHDM:Component()
    return self.loader:Component()
end
