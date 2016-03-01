import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import MenuNodeCreate from '../Menu/MenuNodeCreate.jsx';
import MenuNodeList from '../Menu/MenuNodeList.jsx';

export default class NodeListCreate extends React.Component {
    constructor(props) {
        super(props);

        this.store = this.props.store;
        this.action = this.props.action;
        this.left = 200;
        this.top = 200;

        if(this.props.position && this.props.position.length > 0){
            this.left = this.props.position[0];
            this.top = this.props.position[1];
        }

        this.styles = this.styles.bind(this);
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "rgba(16, 96, 8, 0.5)",
                color: "#eee",
                fontSize: "10pt",
                margin: "0px",
                padding: "0px",
                width: "200px",
                height: "200px",
                position: "absolute",
                top: this.top + "px",
                left: this.left + "px",
                zIndex: "9999",
                float: "left"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.menuArea}>
                <MenuNodeCreate
                    store={this.props.store}
                    action={this.props.action}
                    focusFunction={this.props.focusFunction}
                    ref="creator"
                />
                <MenuNodeList
                    store={this.props.store}
                    action={this.props.action}
                />
            </div>
        );
    }
}
