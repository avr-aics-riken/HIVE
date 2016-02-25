import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Store from './Store.jsx'
import Autosuggest from 'react-autosuggest';

export default class MenuNodeCreate extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
            value: '',
            suggestions: this.getSuggestions('')
        };

        // this.nodeChanged = this.nodeChanged.bind(this);
        // this.selectChanged = this.selectChanged.bind(this);
        // this.moveNode = this.moveNode.bind(this);
        //
        // this.componentDidMount = this.componentDidMount.bind(this);
        // this.componentWillUnmount = this.componentWillUnmount.bind(this);
        // this.onMouseMove = this.onMouseMove.bind(this);
        // this.onMouseUp = this.onMouseUp.bind(this);
        // this.onMouseDown = this.onMouseDown.bind(this);
        // this.onKeyDown = this.onKeyDown.bind(this);
        // this.onKeyUp = this.onKeyUp.bind(this);
        this.styles = this.styles.bind(this);

        this.onChange = this.onChange.bind(this);
        this.onSuggestionsUpdateRequested = this.onSuggestionsUpdateRequested.bind(this);
        this.getSuggestions = this.getSuggestions.bind(this);
        this.getSuggestionValue = this.getSuggestionValue.bind(this);
        this.renderSuggestion = this.renderSuggestion.bind(this);
    }

    // componentDidMount() {
    //     window.addEventListener('mousemove', this.onMouseMove);
    //     window.addEventListener('mouseup', this.onMouseUp);
    //     window.addEventListener('keydown', this.onKeyDown);
    //     window.addEventListener('keyup', this.onKeyUp);
    //     this.props.store.on(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.on(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.on(Store.NODE_MOVED, this.moveNode);
    // }
    //
    // componentWillUnmount() {
    //     window.removeEventListener('mousemove', this.onMouseMove);
    //     window.removeEventListener('mouseup', this.onMouseUp);
    //     window.removeEventListener('keydown', this.onKeyDown);
    //     window.removeEventListener('keyup', this.onKeyUp);
    //     this.props.store.removeListener(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_MOVED, this.moveNode);
    // }

    onChange(event, { newValue }) {
        this.setState({
            value: newValue
        });
    }

    onSuggestionsUpdateRequested({ value }) {
        this.setState({
            suggestions: this.getSuggestions(value)
        });
    }

    getSuggestions(value) {
        var nodeNames = [
            {name: 'kioku'},
            {name: 'kioku_kioku'},
            {name: 'kioku_kioku_kioku'},
            {name: 'uimac'},
            {name: 'uimac_uimac'},
            {name: 'uimac_uimac_uimac'}
        ];
        const inputValue = value.trim().toLowerCase();
        const inputLength = inputValue.length;

        return inputLength === 0 ? [] : nodeNames.filter(nodename =>
             nodename.name.toLowerCase().slice(0, inputLength) === inputValue
        );
    }

    getSuggestionValue(suggestion) { // when suggestion selected, this function tells
        return suggestion.name;                 // what should be the value of the input
    }

    renderSuggestion(suggestion) {
        return (
            <span>{suggestion.name}</span>
        );
    }

    styles() {
        return {
            block: {
                backgroundColor: "#222",
                color: "#eee",
                fontSize: "9pt",
                margin: "0px",
                padding: "0px",
                width: "100%",
                minHeight: "250px",
                position: "absolute",
                bottom: "0px",
                left: "0px"
            },
            theme: {
                // container:                   'react-autosuggest__container',
                // containerOpen:               'react-autosuggest__container--open',
                // input:                       'react-autosuggest__input',
                // suggestionsContainer:        'react-autosuggest__suggestions-container',
                // suggestion:                  'react-autosuggest__suggestion',
                // suggestionFocused:           'react-autosuggest__suggestion--focused',
                // sectionContainer:            'react-autosuggest__section-container',
                // sectionTitle:                'react-autosuggest__section-title',
                // sectionSuggestionsContainer: 'react-autosuggest__section-suggestions-container'
                input: {
                    backgroundColor: "black",
                    color: "whitesmoke",
                    width: "100%",
                    margin: "0px",
                    padding: "3px 0px"
                },
                suggestionsContainer: {
                    listStyle: "none",
                    margin: "0px",
                    padding: "0px",
                    width: "100%",
                },
                suggestion: {
                    borderLeft: "1px solid silver",
                    borderRight: "1px solid silver",
                    borderBottom: "1px solid silver",
                    fontSize: "large",
                    margin: "0px",
                    padding: "5px",
                },
                suggestionFocused: {
                    backgroundColor: "#666"
                }
            }
        }
    }

    render(){
        const style = this.styles();
        const { value, suggestions } = this.state;
        const inputProps = {
            placeholder: 'Type a Node name.',
            value,
            onChange: this.onChange
        };
        return (
            <div style={style.block}>
                <Autosuggest theme={style.theme}
                    suggestions={suggestions}
                    onSuggestionsUpdateRequested={this.onSuggestionsUpdateRequested}
                    getSuggestionValue={this.getSuggestionValue}
                    renderSuggestion={this.renderSuggestion}
                    inputProps={inputProps} />
            </div>
        );
    }
}


