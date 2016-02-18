
console.log('!!createcamera.jsx!!');

import React from 'react'
//var React = require('react');

class CreateCamera extends React.Component {
    constructor(props) {
        super(props);
        console.log("CAMERA CONSTRUCTOR!");
    }

    render(){
        // return (<div>test</div>);
        return (
            //React.createElement('div', {}, 'test create element')
            <div>hoge</div>
        );
    }
}

module.exports = CreateCamera;
