SaveRaw = {}
setmetatable(SaveRaw, {__index = HiveBaseModule})

SaveRaw.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=SaveRaw})
    this.saver = LoadModule("RawVolumeSaver")
    return this
end

function SaveRaw:executeSave()
    local v = self.value
    
    if v.volumedata == nil then
        return false
    end
	self.saver:SetVolumeData(v.volumedata)
	local appendFooter = false
    
    if v.only_export then
        if HIVE_EXPORTMODE then
            return self.saver:Save(v.filepath, appendFooter)
        else
            return true
        end
    else
        return self.saver:Save(v.filepath, appendFooter)
    end
end

function SaveRaw:Do()
    self:UpdateValue()
    
    if mpiMode() == true then
        if mpiRank() == 0 then
            return self:executeSave()
        end
    else
        return self:executeSave()
    end
end

