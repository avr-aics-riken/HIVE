import React from 'react'
import ReactDOM from 'react-dom'

export default class ConsoleOutput extends React.Component {
    constructor(props) {
        super(props);
        this.props = props;        
    }
    styles(show) {
        return {
            position: "absolute",
            backgroundColor: "#F00",
            width: "500px",  
            height: "500px",
            zIndex: "100",
            display: (show ? "block" : "none")
        };
    }
    render () {
        const style = this.styles(this.props.show);
        return (<div style={style}>console</div>);
    }
}
