ParallelCoordCluster = {}
setmetatable(ParallelCoordCluster, {__index = HiveBaseModule})

ParallelCoordCluster.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordCluster})
    this.gentex = GenTexture()
    this.volumeclustering = require('ClusterParallelCoord').VolumeClustering()
    return this
end

function sendData(varname, cdata)
    local mode = 'raw'
    local json = [[{
        "JSONRPC" : "2.0",
        "method" : "renderedImage",
        "to" : ]] .. targetClientId ..[[,
        "param" : {
            "varname": "]] .. varname .. [[",
            "mode": "]] .. mode .. [[",
            "data": ]] .. cdata .. [[
        },
        "id":0
    }]]
    network:SendJSON(json)
end

function ParallelCoordCluster:Do()
    self:UpdateValue()
    self:PrintValue()

    if self.value.volume == nil then
        return 'No volume input'
    end

    print('Clustring = ', self.volumeclustering:Execute(self.value.volume))

    -- dump
    print('---- DUMP -----')
    local axisNum = self.volumeclustering:GetAxisNum()
    local ax
    local temp
    local dest = '{"axis": ['
    print('AxisNum = ' .. axisNum)
    for ax=0, axisNum-1 do
        local cnum = self.volumeclustering:GetClusterNum(ax)
        print('ClusterNum = ' .. cnum)

        -- json string
        if ax == 0 then
            dest = dest .. '{'
        else
            dest = dest .. ',{'
        end
        dest = dest .. '"title": "title_' .. ax .. '", "clusternum": ' .. cnum .. ', "cluster": ['
        for c=0, cnum-1 do
            if c == 0 then
                dest = dest .. '{'
            else
                dest = dest .. ',{'
            end
            local cv = self.volumeclustering:GetClusterValue(ax, c)
            local j = 0
            for i,v in pairs(cv) do
                temp = string.gsub(i, 'Value', '');
                if j ~= 0 then
                    dest = dest .. ','
                end
                j = j + 1
                dest = dest .. '"' .. temp .. '": ' .. v
                -- print(i,v)
            end
            dest = dest .. '}'
        end
        dest = dest .. ']}'
    end

    dest = dest .. ']}'

    print('---- DUMP End ----')

    -- temp
    sendData(self.varname, dest)

    return true
end

function ParallelCoordCluster:select()
    self:UpdateValue()
    return self.gentex:ImageData()
end
