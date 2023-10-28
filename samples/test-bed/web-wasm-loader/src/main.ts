
import Logger from "./Logger";
import { WasmApplication } from "./WasmApplication";
import { extractVarsFromUrl } from "./helpers/index";

const findOrFailHtmlElement = <T extends Element>(elementId: string): T => {
  const textAreaElement = document.querySelector<T>(elementId);
  if (!textAreaElement)
    throw new Error(`DOM elements not found, id=${elementId}`);
  return textAreaElement;
}

const onGlobalPageLoad = async () => {

  let isRunning = true;

  const textAreaElement = findOrFailHtmlElement<HTMLTextAreaElement>("#loggerOutput");
  const logger = new Logger(textAreaElement);

  logger.log("[JS] page loaded");

  const onInitialGlobalPageError = (event: ErrorEvent) => {
    isRunning = false;
    logger.error(`[JS] exception, event=${event}`);
  };
  window.addEventListener("error", onInitialGlobalPageError);

  const errorText = findOrFailHtmlElement<HTMLParagraphElement>("#errorText")!;
  const renderArea = findOrFailHtmlElement<HTMLDivElement>("#renderArea")!;
  const mainCanvas = findOrFailHtmlElement<HTMLCanvasElement>("#canvas")!;

  const hide = (htmlElem: HTMLElement) => {
    if (htmlElem.style.display !== 'none')
      htmlElem.style.display = 'none';
  }
  const show = (htmlElem: HTMLElement) => {
    if (htmlElem.style.display !== 'block')
      htmlElem.style.display = 'block';
  }
  const showErrorText = (htmlText: string) => {
    errorText.innerHTML = htmlText;
    hide(mainCanvas);
    show(errorText);
  };
  const showCanvas = () => {
    hide(errorText);
    show(mainCanvas);
  };
  const resize = (width: number, height: number) => {
    renderArea.style.maxWidth = mainCanvas.style.maxWidth = `${width}px`;
    renderArea.style.maxHeight = mainCanvas.style.maxHeight = `${height}px`;
    mainCanvas.width = width;
    mainCanvas.height = height;
  };

  const onNextGlobalPageError = (event: ErrorEvent) => {
    onInitialGlobalPageError(event);
    showErrorText(`fatal error, event=${event}`);
  };
  window.removeEventListener("error", onInitialGlobalPageError);
  window.addEventListener("error", onNextGlobalPageError);

  //
  //
  // configuration's logic

  const getInteger = (text: string, defaultValue: number): number => text ? parseInt(text, 10) : defaultValue;

  const urlVars = extractVarsFromUrl();
  const config: {
    width: number;
    height: number;
  } = {
    width: getInteger(urlVars.width, 800),
    height: getInteger(urlVars.height, 600),
    // initialMemory: getInteger(urlVars.initialMemory, 128),
  };

  resize(config.width, config.height);

  //
  //
  //

  const onProgress = (percent: number) => {
    const statusMsg = `Loading wasm [${percent}%]`;

    // remove the last logged entry if it was a progress updated
    if (logger.size > 0 && logger.peekLast()!.indexOf("Loading wasm [") >= 0)
      logger.popLast();

    logger.log(`[JS] ${statusMsg}`);
    showErrorText(statusMsg);
  };

  const myApp = new WasmApplication(mainCanvas, onProgress, showErrorText);

  try {

    await myApp.initialize(
      config.width,
      config.height,
      logger,
    );

    showCanvas();
  }
  catch (err) {
    logger.error(`[JS] dependencies check failed: message="${err.message}"`);
    showErrorText([
      `this browser isn't compatible`,
      `error message:`,
      `=> ${err.message}`
    ].join('<br>'));
  }

  //
  //

  // const deltaTime = Math.floor(1000 / 60); // 60fps
  // const onFrameUpdate = () => {
  //   if (isRunning)
  //     setTimeout(onFrameUpdate, deltaTime);

  //   myApp.update(deltaTime);
  // };

  //
  //

  let lastTime = Date.now();
  const onFrameRender = () => {
    if (isRunning)
      requestAnimationFrame(onFrameRender);
      // setTimeout(onFrameRender, deltaTime);

    const currTime = Date.now();
    const deltaTime = currTime - lastTime;
    lastTime = currTime;

    myApp.update(deltaTime);

    myApp.render();
  };

  //
  //

  // onFrameUpdate();
  onFrameRender();
};

window.addEventListener("load", onGlobalPageLoad);
