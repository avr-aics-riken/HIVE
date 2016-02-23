
import React from 'react'

export default class RenderView extends React.Component {
	constructor(props) {
        console.log('RenderView Constructor:', props);
		super(props);
	}
	render() {
		return (<div>view</div>);
	}
}
