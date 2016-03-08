import React from 'react'
import ReactDOM from 'react-dom'

export default class ConsoleOutput extends React.Component {
    constructor(props) {
        super(props);
        this.props = props;
        
        this.state = {
            consoletext: 'This console output'
        }        
        
    }
    
    styles(show) {
        return {
            position: "absolute",
            color: "#CCC",
            backgroundColor: "rgba(50,50,50, 0.8)",
            width: "500px",  
            height: "500px",
            zIndex: "100",
            display: (show ? "block" : "none")
        };
    }
    render () {        
        const style = this.styles(this.props.show);
        return (<div style={style}>{this.state.consoletext}</div>);
    }
}
